// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Avatar.h"
#include "MeleeWeapon.h"
#include "OneHandedSword.h"
#include "Engine/SkeletalMeshSocket.h"


// Constructor

ANPC::ANPC(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

	// Initialise variables
	proximitySphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Proximity Sphere"));
	proximitySphere->SetupAttachment(RootComponent);
	proximitySphere->SetSphereRadius(32.f);

	// Code to make ANMPC::proc() run when this proximity sphere overlaps another actor
	proximitySphere->OnComponentBeginOverlap.AddDynamic(this, &ANPC::proximityCheck);
	
	// Set variables 
	hasBeenHit = false;

	maxHitPoints = 1000;
	currentHitPoints = maxHitPoints;
	attackSpeed = 1.f;
	isBlocking = true;
	wasBlocked = false;

}

// Note, although this was eclared in the header as NPC::Prox() it is now NPC::Prox_Implementaiton here
int ANPC::proximityCheck_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	//This is where our code will fo for what happens when there is an intersection

	// If the overlapped actor is not the player, you should just simply return from the funciton
	if (Cast < AAvatar >(OtherActor) == nullptr) {
		return -1;
	}

	return 0;
}


// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
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


// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
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

}

/* Gettrers and setters */
AMeleeWeapon* const ANPC::getMeleeWeapon() {
	return MeleeWeapon;
}