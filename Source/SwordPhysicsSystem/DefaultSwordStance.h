// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwordStance.h"

/**
 *	DefaultSwordStance class. 
	Inherits from SwordStance such that it can be referenced through its parent pointer and also inherits
	any common functionality.

 	In this stance the user is able to move the avatar freely and also change direction using mouse motion
 */

class AAvatar;

class SWORDPHYSICSSYSTEM_API DefaultSwordStance : public SwordStance
{
private:
	/* Class attributes */

public:

	// Constructor using constructor chaining to call super constructor with inputs
	DefaultSwordStance() {}
	DefaultSwordStance(AAvatar* avatar, int stance_ID) : SwordStance(avatar, stance_ID) {}
	~DefaultSwordStance(); 

	// Virtual function overrides of parent class to redefine functionality specific for this class
	virtual void displayStance() override;
};
