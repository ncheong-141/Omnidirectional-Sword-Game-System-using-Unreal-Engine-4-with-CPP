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
	if (avatarPtr->pController && amount) {

		if (avatarPtr->MeleeWeapon != nullptr) {

			if (avatarPtr->MeleeWeapon->canDamage) {
				avatarPtr->AddControllerYawInput(avatarPtr->baseYawTurnSpeed * amount * avatarPtr->GetWorld()->GetDeltaSeconds());
			}
		}
	}
}

void StabSwordStance::Pitch(float amount) {
	// Change sword position based on mouse position
	//avatarPtr->swordFocalPoint->update(avatarPtr->pController);

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->pController && amount) {

		if (avatarPtr->MeleeWeapon != nullptr) {

			if (avatarPtr->MeleeWeapon->canDamage) {
				avatarPtr->AddControllerPitchInput(avatarPtr->basePitchTurnSpeed * amount * avatarPtr->GetWorld()->GetDeltaSeconds());
			}
		}
	}
}

void StabSwordStance::swordStanceActivation() {

	// Activate attack for stab sword stance
	avatarPtr->isInAttackMotion = true;
	avatarPtr->attackMotionStartingSector = avatarPtr->currentViewportSector->getSectorID();

	// Let weapon know its now attacking
	avatarPtr->MeleeWeapon->startAttackMotion();
}

void StabSwordStance::swordStanceDeactivation() {

	// Deactivate
	avatarPtr->isInAttackMotion = false; 

	// Let weapon know its not attacking
	avatarPtr->MeleeWeapon->endAttackMotion();
}