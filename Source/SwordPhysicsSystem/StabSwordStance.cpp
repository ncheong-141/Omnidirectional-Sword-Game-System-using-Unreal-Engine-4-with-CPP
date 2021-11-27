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

		if (avatarPtr->getMeleeWeapon() != nullptr) {

			if (avatarPtr->getMeleeWeapon()->canDamage) {
				avatarPtr->AddControllerYawInput(avatarPtr->getBaseYawTurnSpeed() * amount * avatarPtr->GetWorld()->GetDeltaSeconds());
			}
		}
	}
}

void StabSwordStance::Pitch(float amount) {
	// Change sword position based on mouse position
	//avatarPtr->swordFocalPoint->update(avatarPtr->pController);

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->pController && amount) {

		if (avatarPtr->getMeleeWeapon() != nullptr) {

			if (avatarPtr->getMeleeWeapon()->canDamage) {
				avatarPtr->AddControllerPitchInput(avatarPtr->getBasePitchTurnSpeed() * amount * avatarPtr->GetWorld()->GetDeltaSeconds());
			}
		}
	}
}

void StabSwordStance::swordStanceActivation() {

	// Activate attack for stab sword stance
	avatarPtr->setAvatarIsInAttackMotion(true);
	avatarPtr->setAttackMotionStartingSector(avatarPtr->getCurrentViewportSector()->getSectorID());
	
	// Set current animation time to 0 
	avatarPtr->animationInstance->currentTime = 0; 

	// Let weapon know its now attacking
	avatarPtr->getMeleeWeapon()->startAttackMotion();
}

void StabSwordStance::swordStanceDeactivation() {

	// Deactivate
	avatarPtr->setAvatarIsInAttackMotion(false);

	// Let weapon know its not attacking
	avatarPtr->getMeleeWeapon()->endAttackMotion();
}