// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "Avatar.h"
#include "NPC.h"



// Sets default values
AMeleeWeapon::AMeleeWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set class attributes 
	avatarWeaponHolder = NULL;
	npcWeaponHolder = NULL;
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
	
	if (weaponHolderIsAvatar) {
		return avatarWeaponHolder; 
	}
	else {
		return npcWeaponHolder;
	}
}

void AMeleeWeapon::setWeaponHolder(AActor* actor) {

	// Set flag for setting if the weapon is held by avatar (just for ease of ref) 
	if (Cast<AAvatar>(actor) != nullptr) {
		avatarWeaponHolder = Cast<AAvatar>(actor);
		weaponHolderIsAvatar = true;
	}
	else {
		npcWeaponHolder = Cast<ANPC>(actor);
		weaponHolderIsAvatar = false;
	}
}
