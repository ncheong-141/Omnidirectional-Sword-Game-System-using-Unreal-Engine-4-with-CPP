// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Game classes
#include "SwordStance.h"

/**
 *  BlockSwordStance class.
	Inherits from SwordStance such that it can be referenced through its parent pointer and also inherits
	any common functionality.

	In this stance the user is able to:
	-	move their sword freely using mouse motion.
	-	Block incoming attacks
	-	Move using WASD but movement is decreased during blocking
 */

// Forward declaration
class AAvatar; 

class SWORDPHYSICSSYSTEM_API BlockSwordStance : public SwordStance
{

private:
	/* Class attributes */
	bool canTurnCamera; 

public:
	
	// Constructor using constructor chaining to call super constructor with inputs
	BlockSwordStance();
	BlockSwordStance(AAvatar* avatar, int stance_ID, bool applyRotationToSFP, bool lockOn_Target);

	// Destructor
	~BlockSwordStance();

	// Virtual function overrides of parent class to redefine functionality specific for this class
	virtual void displayStance() override;

	// Overiding movement functions
	virtual void MoveForward(float amount) override;
	virtual void MoveBack(float amount) override;
	virtual void MoveLeft(float amount) override;
	virtual void MoveRight(float amount) override;

	// Mouse motion override functions for this specific stance
	virtual void Yaw(float amount) override;
	virtual void Pitch(float amount) override;

	// Pure virtual function impl.
	virtual void swordStanceActivation() override;
	virtual void swordStanceDeactivation() override;
	virtual void exitStance() override;
};
