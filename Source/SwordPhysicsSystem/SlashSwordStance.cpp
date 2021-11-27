// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashSwordStance.h"
#include "Avatar.h"
#include "DebugOutput.h"

// Constructor and destructor implementation

SlashSwordStance::~SlashSwordStance()
{
}

// Virtual function override class function implementation
void SlashSwordStance::displayStance() {

	DebugOutput output = DebugOutput();

	output.toHUD(FString("Slash Stance"), 2.f, false);
}


void SlashSwordStance::Yaw(float amount) {

	// Change sword position based on mouse position
	//avatarPtr->swordFocalPoint->update(avatarPtr->pController);
}

void SlashSwordStance::Pitch(float amount) {
	// Change sword position based on mouse position
	//avatarPtr->swordFocalPoint->update(avatarPtr->pController);
}

void SlashSwordStance::calculateAllowableSwordDirections() {

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

void SlashSwordStance::swordStanceActivation() {
	
	// Activate attack for slash sword stance
	avatarPtr->setAvatarIsInAttackMotion(true); 
	avatarPtr->setAttackMotionStartingSector(avatarPtr->getCurrentViewportSector()->getSectorID());

	// Set current animation time to 0 
	avatarPtr->animationInstance->currentTime = 0;

	// Let weapon know its now attacking
	avatarPtr->getMeleeWeapon()->startAttackMotion(); 

	// Activate the recording of mouse delta distances (required for tracking of slash movemment)
	avatarPtr->getSwordFocalPoint()->setRecordMouseDeltaDistances(true);
}

void SlashSwordStance::swordStanceDeactivation() {

	// Deactivate attack motion 
	avatarPtr->setAvatarIsInAttackMotion(false);

	// Let weapon know its not attacking
	avatarPtr->getMeleeWeapon()->endAttackMotion();

	// Reset allowable sword directions for next slash
	allowableSwordDirections.canMoveEast = true;
	allowableSwordDirections.canMoveWest = true;
	allowableSwordDirections.canMoveNorth = true;
	allowableSwordDirections.canMoveSouth = true;

	// Deactivate the recording of mouse delta distances (required for tracking of slash movemment)
	avatarPtr->getSwordFocalPoint()->setRecordMouseDeltaDistances(false);
}