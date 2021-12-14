// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SPSWeaponHolder.generated.h"

class AMeleeWeapon; 

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USPSWeaponHolder : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SWORDPHYSICSSYSTEM_API ISPSWeaponHolder
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Variable setting functions, forcing implementing object to have these variables
	
	// Blocking variables 
	virtual bool SPSActorIsBlocking() = 0;
	virtual bool SPSActorWasBlocked() = 0;
	virtual void SPSSetActorIsBlocking(bool value) = 0;
	virtual void SPSSetActorWasBlocked(bool value) = 0;
	virtual void stopAttackIfBlocked() = 0; 

	// Hit point variables 
	virtual float SPSActorGetHP() = 0;
	virtual float SPSActorGetMaxHP() = 0;
	virtual void SPSActorTakeDamage(float amount) = 0;

	// Weapon 
	virtual AMeleeWeapon* const getMeleeWeapon() = 0; 

	// Attack speed/reversing 
	virtual float getAttackSpeed() = 0;
	virtual void setAttackSpeed(float amount) = 0; 

};
