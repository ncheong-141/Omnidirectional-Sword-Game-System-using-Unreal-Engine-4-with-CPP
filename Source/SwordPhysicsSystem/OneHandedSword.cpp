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
	proximityBox->SetupAttachment(RootComponent);
}

void AOneHandedSword::init(FString name_, float mass_) {
	
	// Initialise variables
	this->name = name_; 
	this->mass = mass_; 
}



int AOneHandedSword::proximityCheck_Implementation(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {

	// Dont hit non-root components 
	//if (otherComp != otherActor->GetRootComponent()) {
	//	return -1;
	//}

	// Check if other actor is a sword
	AMeleeWeapon* enemySword = Cast<AMeleeWeapon>(otherActor);
	if (enemySword != nullptr) {

		// Check if sword has been hit before. Does not need to be in the can damage state but needs to be in attack motion
		if (!targetsHit.Contains(otherActor) && weaponHolderInterfaceReference->SPSActorIsInAttackMotion()) {
			
			UE_LOG(LogTemp, Display, TEXT("Hit a enemy sword."));

			// Add ssword to targets hit to not hit more than once in one attack
			targetsHit.Add(otherActor);

			// Cast targetable object to targetable interface
			ISPSWeaponHolder* enemySwordOwner = Cast<ISPSWeaponHolder>(enemySword->getWeaponHolder());

			// Check cast is successful/ enemy is targetable
			if (enemySwordOwner) {

				// Check if other sword is blocking
				if (enemySwordOwner->SPSActorIsBlocking()) {

					// Blocking, so set this attacking actor to WasBlocked
					weaponHolderInterfaceReference->SPSSetActorWasBlocked(true);
					UE_LOG(LogTemp, Display, TEXT("Enemy Sword is blocking and set Attacker to was blocked"));
				}
			}

			// If hit enemy sword, dont consider any other targets
			
			return 0;;
		}

	}

	// Check if an Targatable SPS actor ( check if cast can be succssful)
	if (Cast<ISPSWeaponHolder>(otherActor)) {

		// If the weapon holder is an Avatar
		if (weaponHolderIsAvatar) {

			// Dont hit things when conditions arent met
			if (canDamage && otherActor != (AActor*)weaponHolder && !targetsHit.Contains(otherActor)) {

				// Damage actor

				// Add to hit list so do not strike more than once with one swing
				targetsHit.Add(otherActor);

				// Set staggared if target is a NPC 
				ANPC* npc = Cast<ANPC>(otherActor);

				// Check if cast successful
				if (npc != nullptr) {

					// Set hasBeenHit to true only if its not already in animation
					if (npc->getHasBeenHit() == false) {
						npc->setHasBeenHit(true);
					}
				}
			}
		}
		// Should be done through the interface 
		else {
			// Not an avatar but NPC (
			
			// Dont hit things when conditions arent met
			// Removed: avatarWeaponHolder->avatarIsInAttackMotion() to generalise and canDamage is never true unless in attack motion
			if (canDamage && otherActor != (AActor*)weaponHolder && !targetsHit.Contains(otherActor)) {

				// Set staggared if target is a NPC 
				AAvatar* avatar = Cast<AAvatar>(otherActor);


				// Check if cast successful
				if (avatar != nullptr) {

					// Check to see if in iframe
					if (!avatar->avatarIsInIframe()) {

						// Damage actor

						// Add to hit list so do not strike more than once with one swing
						targetsHit.Add(otherActor);

						// Set hasBeenHit to true only if its not already in animation
						if (avatar->getHasBeenHit() == false) {
							avatar->setHasBeenHit(true);
						}
					}

				}


			}
		}
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

		//Cast to avatar
		AAvatar* avatar = Cast<AAvatar>(weaponHolder);
		// Change right hand resultant speed to + movement speed later
		return mass * avatar->getRighthandResultantSpeed() * AMeleeWeapon::canDamage;
	}
	else {
		// NPC damage
		return 1; 
	}
}