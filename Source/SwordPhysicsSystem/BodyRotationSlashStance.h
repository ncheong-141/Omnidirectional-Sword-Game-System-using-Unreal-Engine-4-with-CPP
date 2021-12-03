// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SWORDPHYSICSSYSTEM_API BodyRotationSlashStance : public SwordStance
{
private:
	/* Class attributes */

public:

	// Constructor using constructor chaining to call super constructor with inputs
	BodyRotationSlashStance();
	BodyRotationSlashStance(AAvatar* avatar, int stance_ID, bool applyRotationToSFP);
	~BodyRotationSlashStance();

	// Virtual function overrides of parent class to redefine functionality specific for this class
	virtual void displayStance() override;

	// Mouse motion override functions for this specific stance
	virtual void Yaw(float amount) override;
	virtual void Pitch(float amount) override;

	// Virtual function impl.
	virtual void calculateAllowableSwordDirections() override;

	// Pure virtual function impl.
	virtual void swordStanceActivation() override;
	virtual void swordStanceDeactivation() override;
};
