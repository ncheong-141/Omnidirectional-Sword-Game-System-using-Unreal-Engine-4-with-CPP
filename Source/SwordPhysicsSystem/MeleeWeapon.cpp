// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "Avatar.h"
#include "NPC.h"



// Sets default values
AMeleeWeapon::AMeleeWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set class attributes 
	weaponHolder = nullptr;
	targetsHit = TSet<AActor*>();
	canDamage = false;
	weaponHolderIsAvatar = false; 
}


// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AMeleeWeapon::calculateDynamicDamage() {
	return 0.f;
}

void AMeleeWeapon::startAttackMotion() {
	// Empty the target list for next swing
	targetsHit.Empty();
}

void AMeleeWeapon::endAttackMotion() {
	// Empty the target list for next swing
	targetsHit.Empty();
}


// Getters and setters
TSet<AActor*> AMeleeWeapon::getTargetsHit() {
	return targetsHit;
}

AActor* AMeleeWeapon::getWeaponHolder() {
	
	return weaponHolder;
}

void AMeleeWeapon::setWeaponHolder(AActor* actor) {

	// Set weapon holder
	weaponHolder = actor; 

	// Set flag for setting if the weapon is held by avatar (just for ease of ref) 
	if (Cast<AAvatar>(actor) != nullptr) {
		weaponHolderIsAvatar = true;
	}
	else {
		weaponHolderIsAvatar = false;
	}

	// Set up interface reference for future usage
	weaponHolderInterfaceReference = TScriptInterface<ISPSTargetable>(actor);

	 //Check
	if (weaponHolderInterfaceReference == nullptr) {
		UE_LOG(LogTemp, Fatal, TEXT("Weapon holder is not a SPSTargetable interfaced object in %s"), __FUNCTION__);
	}

}
