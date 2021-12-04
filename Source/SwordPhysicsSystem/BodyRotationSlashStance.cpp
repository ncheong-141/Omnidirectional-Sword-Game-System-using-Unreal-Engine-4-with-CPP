// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyRotationSlashStance.h"

#include "SwordFocalPoint.h"
#include "Avatar.h"
#include "DebugOutput.h"

// Game classes
#include "GameFramework/CharacterMovementComponent.h"


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

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->pController && amount) {
		
		// Used to slow the yaw motion when not attacking
		float stanceYawFactor = 0.1; 
		
		// Avatar is attacking
		if (avatarPtr->avatarIsInAttackMotion()) {
			
			// Set avatar to now rotate with camera yaw
			avatarPtr->bUseControllerRotationYaw = true;
			avatarPtr->avatarMovementComponent->bOrientRotationToMovement = false;
			avatarPtr->AddControllerYawInput(avatarPtr->getBaseYawTurnSpeed() * amount * avatarPtr->GetWorld()->GetDeltaSeconds());
		}
		else {
			avatarPtr->AddControllerYawInput(avatarPtr->getBaseYawTurnSpeed() *stanceYawFactor* amount * avatarPtr->GetWorld()->GetDeltaSeconds());
		}
	}
}

void BodyRotationSlashStance::Pitch(float amount) {
	// Change sword position based on mouse position
	//avatarPtr->swordFocalPoint->update(avatarPtr->pController);
}

void BodyRotationSlashStance::calculateAllowableSwordDirections() {

	// BRSS Stance rule: Is rotating so only makjes sense to move sword up and down
	// Needs to be minimised though 
	// 
	// X axis
	allowableSwordDirections.canMoveEast = false;
	allowableSwordDirections.canMoveWest = false;
	
	// Y axis
	// Refer to swordfocalpoint object  
	allowableSwordDirections.canMoveNorth = true;
	allowableSwordDirections.canMoveSouth = true;

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