// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Avatar.h"
#include "MeleeWeapon.h"
#include "OneHandedSword.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"


// External library for random
#include <stdlib.h> 

// Constructor

ANPC::ANPC(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

	// Initialise variables
	proximitySphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Proximity Sphere"));
	proximitySphere->SetupAttachment(RootComponent);
	proximitySphere->SetSphereRadius(500.f);

	attackRangeSize = 200.f;
	attackRangeSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Attack Range Sphere"));
	attackRangeSphere->SetupAttachment(RootComponent);
	attackRangeSphere->SetSphereRadius(attackRangeSize);

	// Code to make ANMPC::proc() run when this proximity sphere overlaps another actor
	proximitySphere->OnComponentBeginOverlap.AddDynamic(this, &ANPC::proximityCheck);
	proximitySphere->OnComponentEndOverlap.AddDynamic(this, &ANPC::endProximityCheck);
	
	// Attack range sphere functionality
	attackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ANPC::attackRangeCheck);
	attackRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ANPC::endAttackRangeCheck);

	
	// Set variables 
	maxHitPoints = 1000;
	currentHitPoints = maxHitPoints;
	attackSpeed = 1.f;
	maxMovementSpeed = GetCharacterMovement()->GetMaxSpeed();
	worldVelocity = FVector2D(0.f);
	localVelocity = FVector2D(0.f);
	normalisedLocalVelocity = FVector2D(0.f);

	// AI Control Variables
	rotateToAvatar = true;
	moveToAvatar = false;
	attackAvatar = false; 
	blockForAvatar = false;

	// AI flags
	avatarInProximity = false;
	moving = false;
	attacking = false;
	isBlocking = false;
	wasBlocked = false;
	hasBeenHit = false;

	// Attacking system
	numAttacksAvailable = 14; 
	currentAttackID = 0;

	// Blocking system
	npcBasicSwordFocalPoint = FVector2D(0.f);

}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update velocities
	velocityUpdate();

	// If avatar is in proximity
	if (avatarInProximity) {
		
		// Turn to avatar if flagged to 
		if (rotateToAvatar) {
			turnToAvatar();
		}

		// If move to avatar
		if (moveToAvatar) {
			moveTowardsAvatar(DeltaTime);
		}

		// If attack avatar
		if (attackAvatar) {
			startAttackingAvatar();
		}

		// If is blocking
		if (isBlocking) {
			putGuardUp();
		}
	}
}


// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	// Get avatar reference
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (playerPawn) {
		avatarRef = Cast<AAvatar>(playerPawn);
	}
}

void ANPC::PostInitializeComponents() {

	Super::PostInitializeComponents();

	// Instantiate the melee weapon if a BP was selected in UE4
	if (BPMeleeWeapon) {

		// Instantiate melee weapon (Onehanded sword here, need a check for what is in BPMeleewapon)
		MeleeWeapon = GetWorld()->SpawnActor<AOneHandedSword>(BPMeleeWeapon, FVector(), FRotator());

		// If instantiation was successful, apply it to avatar
		if (MeleeWeapon) {

			// Get refence to the socket
			const USkeletalMeshSocket* socket = GetMesh()->GetSocketByName(FName("hand_rSocket"));

			if (socket) {
				// Attach meleeWeapon to socket
				socket->AttachActor(MeleeWeapon, GetMesh());
				MeleeWeapon->setWeaponHolder(this);
			}
		}
	}
}

void ANPC::turnToAvatar() {

	if (avatarRef != nullptr) {

		// actor location - location = vector pointing to avatar
		FVector vectorToAvatar = avatarRef->GetActorLocation() - GetActorLocation();

		// Get rotation of the vector
		FRotator rotationToAvatar = vectorToAvatar.Rotation(); 

		// set the pitch to 0 as dont want npc to pitch
		rotationToAvatar.Pitch = 0; 

		// Set world rotation of NPC (Hard lock on) 
		RootComponent->SetWorldRotation(rotationToAvatar); 
	}
}

void ANPC::moveTowardsAvatar(float deltaSeconds) {
	
	if (avatarRef != nullptr) {

		// actor location - location = vector pointing to avatar
		FVector vectorToAvatar = avatarRef->GetActorLocation() - GetActorLocation();

		float sizeVec = vectorToAvatar.Size(); 

		// Change to unit vector
		vectorToAvatar.Normalize(); 

		// Add input to movment of NPC (dont walk right up)
		if (sizeVec > 0.5* attackRangeSize) {
			AddMovementInput(vectorToAvatar, maxMovementSpeed * deltaSeconds);
		}
	}
}

void ANPC::startAttackingAvatar() {

	// In attack range, start an attack if not already attacking 
	if (attacking && !inAttackMotion) {

		// Generate a random value between 0 - Number of attacks
		currentAttackID = rand() % numAttacksAvailable;

		// Executes the attack from anim blueprint
		inAttackMotion = true;

		// Set Weapon in attack
		MeleeWeapon->startAttackMotion();

		// This gets set back to false in anim notif state for NPC
	}
}

void ANPC::putGuardUp() {

	// Track Avatar focal point if he/she is not attacking, when attacking dont move sword
	// As then this will be impossible to hit
	// Could do a step towards Avatar focal point but maybe later
	if (!avatarRef->avatarIsInAttackMotion()) {

		// Get player focal point
		FVector2D avatarSFPPosition = avatarRef->getSwordFocalPoint()->position2D;

		// Need to be in opposites sides in X but same in Y
		npcBasicSwordFocalPoint.X = 1.f - avatarSFPPosition.X;
		npcBasicSwordFocalPoint.Y = avatarSFPPosition.Y;
	}
}

void ANPC::velocityUpdate() {

	// Calculate the local velocity of the avatar from the World velocity
	FVector npcWorldVelocity = this->GetVelocity();
	FQuat	npcWorldRotation = this->GetActorTransform().GetRotation();
	FVector npcLocalVelocity = npcWorldRotation.UnrotateVector(npcWorldVelocity);

	// Set world velocity
	worldVelocity.X = npcWorldVelocity.X;
	worldVelocity.Y = npcWorldVelocity.Y;

	// Set local/relative velocity
	localVelocity.X = npcLocalVelocity.X;
	localVelocity.Y = npcLocalVelocity.Y;

	// Calculate the normalised inputed velocity (this is currently wrong)
	maxMovementSpeed = this->GetCharacterMovement()->GetMaxSpeed();
	normalisedLocalVelocity.X = npcLocalVelocity.X / maxMovementSpeed;
	normalisedLocalVelocity.Y = npcLocalVelocity.Y / maxMovementSpeed;
}




// Note, although this was eclared in the header as NPC::ProximityCheck() it is now NPC::ProximityCheck_Implementaiton here
int ANPC::proximityCheck_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	//This is where our code will fo for what happens when there is an intersection

	// If the overlapped actor is not the player, you should just simply return from the funciton
	if (Cast < AAvatar >(OtherActor) == nullptr) {
		return -1;
	}

	// Avatar entered prximity
	avatarInProximity = true;
	return 0;
}

int ANPC::endProximityCheck_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	// If the overlapped actor is not the player, you should just simply return from the funciton
	if (Cast < AAvatar >(OtherActor) == nullptr) {
		return -1;
	}

	// Avatar exited prximity
	avatarInProximity = false;
	return 0;
}


// Attack range sphere functionality
int ANPC::attackRangeCheck_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	//This is where our code will fo for what happens when there is an intersection

	// If the overlapped actor is not the player, you should just simply return from the funciton
	if (Cast < AAvatar >(OtherActor) == nullptr) {
		return -1;
	}

	// Avatar entered attack range
	attacking = true;

	return 0;
}

int ANPC::endAttackRangeCheck_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	// If the overlapped actor is not the player, you should just simply return from the funciton
	if (Cast < AAvatar >(OtherActor) == nullptr) {
		return -1;
	}

	// Avatar exited attack range
	attacking = false;

	return 0;
}





/* Getters and setters */
bool ANPC::getHasBeenHit() {
	return hasBeenHit;
}

void ANPC::setHasBeenHit(bool value) {
	hasBeenHit = value;
}

bool ANPC::getIsInAttackMotion() {
	return inAttackMotion;
}

void ANPC::setIsInAttackMotion(bool value) {
	inAttackMotion = value;
}



/* Targetable Interface functions*/
bool ANPC::SPSActorIsBlocking() {
	return isBlocking;
}

bool ANPC::SPSActorWasBlocked() {
	return wasBlocked;
}


void ANPC::SPSSetActorIsBlocking(bool value) {
	isBlocking = value;
}

void ANPC::SPSSetActorWasBlocked(bool value) {
	wasBlocked = value;
}

float ANPC::SPSActorGetHP() {
	return currentHitPoints;
}

float ANPC::SPSActorGetMaxHP() {
	return maxHitPoints;
}

void ANPC::SPSActorTakeDamage(float amount) {

	// take damage
	currentHitPoints -= amount;

	// Check if dead etc 
}

float ANPC::getAttackSpeed() {
	return attackSpeed;
}
void ANPC::setAttackSpeed(float amount) {
	attackSpeed = amount;
}


void ANPC::stopAttackIfBlocked() {
	inAttackMotion = false;
}

/* Gettrers and setters */
AMeleeWeapon* const ANPC::getMeleeWeapon() {
	return MeleeWeapon;
}