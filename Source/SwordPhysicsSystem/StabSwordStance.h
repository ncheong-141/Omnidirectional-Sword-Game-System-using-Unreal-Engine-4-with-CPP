// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwordStance.h"

/**
 *  StabSwordStance class.
	Inherits from SwordStance such that it can be referenced through its parent pointer and also inherits
	any common functionality.

	In this stance the user is able to:
	-	move their sword freely using mouse motion.
	-	The user should also be able to move their character using WASD
 */

// Forward declaration
class AAvatar; 

class SWORDPHYSICSSYSTEM_API StabSwordStance : public SwordStance
{
private:
	/* Class attributes */
public:

	// Constructor using constructor chaining to call super constructor with inputs
	StabSwordStance();
	StabSwordStance(AAvatar* avatar, int stance_ID, bool applyRotationToSFP, bool lockOn_Target);
	// Destructor
	~StabSwordStance();

	// Virtual function overrides of parent class to redefine functionality specific for this class
	virtual void displayStance() override;

	// Mouse motion override functions for this specific stance
	virtual void Yaw(float amount) override; 
	virtual void Pitch(float amount) override;

	// Pure virtual function impl.
	virtual void swordStanceActivation() override;
	virtual void swordStanceDeactivation() override;
	virtual void exitStance() override;
};
