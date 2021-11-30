// Fill out your copyright notice in the Description page of Project Settings.

#include "StabSwordStance.h"
#include "Avatar.h"
#include "GameFramework/CharacterMovementComponent.h"

// Debug output file
#include "DebugOutput.h"

// Constructors and destructor
StabSwordStance::StabSwordStance() {

}

StabSwordStance::StabSwordStance(AAvatar* avatar, int stance_ID, bool applyRotationToSFP) : SwordStance(avatar, stance_ID, applyRotationToSFP) {

}

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

		if (avatarPtr->getMeleeWeapon() != nullptr && avatarPtr->avatarIsInAttackMotion()) {

			if (avatarPtr->getMeleeWeapon()->canDamage) {

				// Switch to avatar following camera angle
				avatarPtr->bUseControllerRotationYaw = true;
				avatarPtr->avatarMovementComponent->bOrientRotationToMovement = false;
				avatarPtr->AddControllerYawInput(avatarPtr->getBaseYawTurnSpeed() * amount * avatarPtr->GetWorld()->GetDeltaSeconds());
			}
			else {
				avatarPtr->bUseControllerRotationYaw = false;
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
	
	// Set the stance activation to true
	stanceActivationJustStarted = true;
	stanceActivated = true;

	// Let weapon know its now attacking
	avatarPtr->getMeleeWeapon()->startAttackMotion();
}

void StabSwordStance::swordStanceDeactivation() {

	// Deactivate
	avatarPtr->setAvatarIsInAttackMotion(false);

	// Set the stance activation to false
	stanceActivated = false;
	stanceActivationJustEnded = true;

	// Let weapon know its not attacking
	avatarPtr->getMeleeWeapon()->endAttackMotion();
}