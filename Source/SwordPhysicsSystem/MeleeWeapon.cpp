// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "Avatar.h"



// Sets default values
AMeleeWeapon::AMeleeWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set class attributes 
	weaponHolder = NULL;
	targetsHit = TSet<AActor*>();
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

AAvatar* AMeleeWeapon::getWeaponHolder() {
	return weaponHolder;
}

void AMeleeWeapon::setWeaponHolder(AAvatar* avatar) {
	weaponHolder = avatar;
}
