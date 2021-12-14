// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPSTargetable.h"
#include "MeleeWeapon.generated.h"

// Forward declaration
class AAvatar; 
class ANPC;
class UBoxComponent;

/*
* Abstract class used to define melee weapon implemenmtation
* Onehanded sword derives from this. Can also give other weapon types similar functionality
*/

UCLASS()
class SWORDPHYSICSSYSTEM_API AMeleeWeapon : public AActor
{
	GENERATED_BODY()

protected:
	
	/* Abstract class variables */
	// A list of things the meleeweapon already hit this swing
	// Ensures target is only hit once per swing
	UPROPERTY()
		TSet<AActor*> targetsHit; 
	
	// Reference to weapon holder to check ensure does not hit themselve
	// Two references as will set, one is the interface reference for quick access
	UPROPERTY()
		AActor* weaponHolder;
	UPROPERTY()
		TScriptInterface<ISPSTargetable> weaponHolderInterfaceReference; 

	bool weaponHolderIsAvatar; 

	// UE4 functions 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/* Public Abstract class variables */
	// Can damage variable, toggled depending on what part of the animation the sword is in 
	// This is set in animation notifications
	bool canDamage; 

	// Constructor
	AMeleeWeapon(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/* Class member functions/UE4 functions */

	// Virtual functions which sub classes can implement
	// Need to change the parent class from actor to make this a interface...
	virtual float calculateDynamicDamage();
	virtual void startAttackMotion();
	virtual void endAttackMotion();

	// Getters and Setters
	TSet<AActor*> getTargetsHit();
	AActor* getWeaponHolder();
	void setWeaponHolder(AActor* actor);
};
