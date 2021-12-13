// Fill out your copyright notice in the Description page of Project Settings.


#include "OneHandedSword.h"
#include "Avatar.h"
#include "Components/BoxComponent.h"
#include "NPC.h"


// Constructor

AOneHandedSword::AOneHandedSword(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	name = FString(); 
	mass = 0.f; 
	dynamicDamage = 0.f;

	// Create mesh and set as root compoonent
	mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("mesh"));

	// Note, root component is basically meant to be the core of the object and defines how you collide wioththe object
	// UE4 intends rootcomponent to always be referenced to the collision primitive
	RootComponent = mesh;

	// Create proximity box
	proximityBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("proximityBox"));

	// Add dynamic/functionality to what happens in this proximity box overlaps another actor
	proximityBox->OnComponentBeginOverlap.AddDynamic(this, &AOneHandedSword::proximityCheck);

	// Attach proximity box to the root component (the mesh here)
	proximityBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}

void AOneHandedSword::init(FString name_, float mass_) {
	
	// Initialise variables
	this->name = name_; 
	this->mass = mass_; 
}



int AOneHandedSword::proximityCheck_Implementation(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {

	// Dont hit non-root components 
	if (otherComp != otherActor->GetRootComponent()) {
		return -1;
	}

	// If the weapon holder is an Avatar
	if (weaponHolderIsAvatar) {
		
		// Dont hit things when conditions arent met
		if (avatarWeaponHolder->avatarIsInAttackMotion() && canDamage && otherActor != (AActor*)avatarWeaponHolder && !targetsHit.Contains(otherActor)) {

			// Damage actor
			otherActor->TakeDamage(calculateDynamicDamage(), FDamageEvent(), NULL, this);
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, FString::Printf(TEXT("Target Hit!")));

			// Add to hit list so do not strike more than once with one swing
			targetsHit.Add(otherActor);

			// Set staggared if target is a NPC 
			ANPC* npc = Cast<ANPC>(otherActor);

			// Check if cast successful
			if (npc != nullptr) {

				npc->hasBeenHit = true;
			}
		}
	}
	else {
		// Not an avatar but NPC (

	}

	return 0;
}



void AOneHandedSword::startAttackMotion() {

	// Empty the target list for next swing
	targetsHit.Empty();
}

void AOneHandedSword::endAttackMotion() {
	targetsHit.Empty();
}

// Calculate damage based on sword movementspeed and weigt
float AOneHandedSword::calculateDynamicDamage() {

	// Check if is an avatar
	if (weaponHolderIsAvatar) {
		// Change right hand resultant speed to + movement speed later
		return mass * avatarWeaponHolder->getRighthandResultantSpeed() * AMeleeWeapon::canDamage;
	}
	else {
		// NPC damage
		return 1; 
	}
}