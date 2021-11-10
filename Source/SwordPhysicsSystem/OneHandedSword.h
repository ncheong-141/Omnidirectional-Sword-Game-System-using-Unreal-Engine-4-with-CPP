// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeWeapon.h"
#include "OneHandedSword.generated.h"

/**
 * 
 */
UCLASS()
class SWORDPHYSICSSYSTEM_API AOneHandedSword : public AMeleeWeapon
{
	GENERATED_BODY()

public:

	/* Sword attributes */

	/* Constructor */
	AOneHandedSword(const FObjectInitializer& ObjectInitializer);

	/* Class member functions */

	// virtual function override
	virtual void swing() override;
	virtual void rest() override;
};
