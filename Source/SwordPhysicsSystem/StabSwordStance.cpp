// Fill out your copyright notice in the Description page of Project Settings.

#include "StabSwordStance.h"
#include "Avatar.h"

// Debug output file
#include "DebugOutput.h"

StabSwordStance::~StabSwordStance()
{
}

// Virtual function override class function implementation
void StabSwordStance::displayStance() {

	DebugOutput output = DebugOutput();

	output.toHUD(FString("Stab Stance"), 2.f, false);
}


void StabSwordStance::Yaw(float amount) {

	// Change sword position based on mouse position
	//avatarPtr->swordFocalPoint->update(avatarPtr->pController);

	// Allow for camera angling when attacking
	if (avatarPtr->pController && amount && avatarPtr->isInAttackMotion) {

		avatarPtr->AddControllerYawInput(avatarPtr->baseYawTurnSpeed * amount * avatarPtr->GetWorld()->GetDeltaSeconds());

	}
}

void StabSwordStance::Pitch(float amount) {
	// Change sword position based on mouse position
	//avatarPtr->swordFocalPoint->update(avatarPtr->pController);

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->pController && amount && avatarPtr->isInAttackMotion) {

		avatarPtr->AddControllerPitchInput(avatarPtr->basePitchTurnSpeed * amount * avatarPtr->GetWorld()->GetDeltaSeconds());
	}
}

void StabSwordStance::swordStanceActivation() {

	// Activate attack for stab sword stance
	avatarPtr->isInAttackMotion = true;
	avatarPtr->attackMotionStartingSector = avatarPtr->currentViewportSector->getSectorID();
}

void StabSwordStance::swordStanceDeactivation() {

	// Deactivate
	avatarPtr->isInAttackMotion = false; 
}