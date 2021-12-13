// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashSwordStance.h"
#include "Avatar.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AvatarHUD.h"

// Constructor and destructor implementation
SlashSwordStance::SlashSwordStance() {

}

SlashSwordStance::SlashSwordStance(AAvatar* avatar, int stance_ID, bool applyRotationToSFP, bool lockOn_Target) : SwordStance(avatar, stance_ID, applyRotationToSFP, lockOn_Target) {

}

SlashSwordStance::~SlashSwordStance()
{
}

// Virtual function override class function implementation
void SlashSwordStance::displayStance() {

	avatarPtr->avatarHUD->addHUDMessage(HUDMessage("Slash Stance", 2.f, FColor::White));

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

	// Set the stance activation to true
	stanceActivationJustStarted = true;
	stanceActivated = true; 

	// Let weapon know its now attacking
	avatarPtr->getMeleeWeapon()->startAttackMotion(); 

	// Activate the recording of mouse delta distances (required for tracking of slash movemment)
	avatarPtr->getSwordFocalPoint()->setRecordMouseDeltaDistances(true);
}

void SlashSwordStance::swordStanceDeactivation() {

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

	// Deactivate the recording of mouse delta distances (required for tracking of slash movemment)
	avatarPtr->getSwordFocalPoint()->setRecordMouseDeltaDistances(false);
}