// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyRotationSlashStance.h"

#include "Avatar.h"
#include "DebugOutput.h"

// Constructor and destructor implementation
BodyRotationSlashStance::BodyRotationSlashStance() {

}

BodyRotationSlashStance::BodyRotationSlashStance(AAvatar* avatar, int stance_ID, bool applyRotationToSFP) : SwordStance(avatar, stance_ID, applyRotationToSFP) {

}

BodyRotationSlashStance::~BodyRotationSlashStance()
{
}

// Virtual function override class function implementation
void BodyRotationSlashStance::displayStance() {

	DebugOutput output = DebugOutput();

	output.toHUD(FString("BodyRotationSlashStance Stance"), 2.f, false);
}


void BodyRotationSlashStance::Yaw(float amount) {

	// Change sword position based on mouse position
	//avatarPtr->swordFocalPoint->update(avatarPtr->pController);
}

void BodyRotationSlashStance::Pitch(float amount) {
	// Change sword position based on mouse position
	//avatarPtr->swordFocalPoint->update(avatarPtr->pController);
}

void BodyRotationSlashStance::calculateAllowableSwordDirections() {

	// Slash Stance rule: If any of the directions are predominant, change the allowable direction to that

	// Check directions
	// if both axis directions are true, there is no dominant direciton yet.
	// If one is false, then it is set to that until the attack slash is over

	// X axis
	if (allowableSwordDirections.canMoveEast && allowableSwordDirections.canMoveWest) {

		// Refer to swordfocalpoint object  
		allowableSwordDirections.canMoveEast = avatarPtr->getSwordFocalPoint()->isDominantDirectionEast();
		allowableSwordDirections.canMoveWest = avatarPtr->getSwordFocalPoint()->isDominantDirectionWest();
	}

	// Y axis
	if (allowableSwordDirections.canMoveNorth && allowableSwordDirections.canMoveSouth) {

		// Refer to swordfocalpoint object  
		allowableSwordDirections.canMoveNorth = avatarPtr->getSwordFocalPoint()->isDominantDirectionNorth();
		allowableSwordDirections.canMoveSouth = avatarPtr->getSwordFocalPoint()->isDominantDirectionSouth();
	}

	// This is reset when the slash ends
}

void BodyRotationSlashStance::swordStanceActivation() {

	// Activate attack for slash sword stance
	avatarPtr->setAvatarIsInAttackMotion(true);
	avatarPtr->setAttackMotionStartingSector(avatarPtr->getCurrentViewportSector()->getSectorID());

	// Set the stance activation to true
	stanceActivationJustStarted = true;
	stanceActivated = true;

	// Let weapon know its now attacking
	avatarPtr->getMeleeWeapon()->startAttackMotion();
}

void BodyRotationSlashStance::swordStanceDeactivation() {

	// Deactivate attack motion 
	avatarPtr->setAvatarIsInAttackMotion(false);

	// Set the stance activation to false
	stanceActivated = false;
	stanceActivationJustEnded = true;

	// Let weapon know its not attacking
	avatarPtr->getMeleeWeapon()->endAttackMotion();

	// Reset allowable sword directions for next slash
	allowableSwordDirections.canMoveEast = true;
	allowableSwordDirections.canMoveWest = true;
	allowableSwordDirections.canMoveNorth = true;
	allowableSwordDirections.canMoveSouth = true;
}