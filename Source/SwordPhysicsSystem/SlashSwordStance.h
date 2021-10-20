// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwordStance.h"

/**
 *  SlashSwordStance class. 
	Inherits from SwordStance such that it can be referenced through its parent pointer and also inherits
	any common functionality.

	In this stance the user is able to:
	-	move their sword freely using mouse motion. 
	-	The user should also be able to move their character using WASD
 */
class SWORDPHYSICSSYSTEM_API SlashSwordStance : public SwordStance
{
private:
	/* Class attributes */
public:

	// Constructor and destructor 
	SlashSwordStance();
	~SlashSwordStance();

	// Virtual function overrides of parent class to redefine functionality specific for this class
	virtual void displayStance() override;

};
