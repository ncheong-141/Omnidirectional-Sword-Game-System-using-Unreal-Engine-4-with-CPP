// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockSwordStance.h"
#include "Avatar.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AvatarHUD.h"

BlockSwordStance::BlockSwordStance() {

}

BlockSwordStance::BlockSwordStance(AAvatar* avatar, int stance_ID, bool applyRotationToSFP, bool lockOn_Target) : SwordStance(avatar, stance_ID, applyRotationToSFP, lockOn_Target) {

	// Instantiate class attributes
	canTurnCamera = false;
}

BlockSwordStance::~BlockSwordStance()
{
}


// Virtual function override class function implementation
void BlockSwordStance::displayStance() {
	avatarPtr->avatarHUD->addHUDMessage(HUDMessage("Block Stance", 2.f, FColor::White));

}


/* Player input WASD */

void BlockSwordStance::MoveForward(float amount) {

	// Don't enter the body of this function if the controller is not set up, or amount == 0
	if (SwordStance::avatarPtr->pController && amount) {

		// Half the amount when moving
		amount = amount / 2;

		// Get current forward movement
		FVector fwd = SwordStance::avatarPtr->GetActorForwardVector();

		// We call add movement to actually move the player by amount in the fwd direction
		SwordStance::avatarPtr->AddMovementInput(fwd, amount);
	}
}

void BlockSwordStance::MoveBack(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0
	if (SwordStance::avatarPtr->pController && amount) {

		// Half the amount when moving
		amount = amount / 2;

		// Get current forward movement (no back vector) 
		FVector fwd = SwordStance::avatarPtr->GetActorForwardVector();

		// Add amount to movement, since it is back it is subtract
		SwordStance::avatarPtr->AddMovementInput(fwd, -amount);
	}
}

void BlockSwordStance::MoveRight(float amount) {

	// Dont enter the body ofthis function if the controller is not set up, or amount == 0; 
	if (SwordStance::avatarPtr->pController && amount) {

		// Half the amount when moving
		amount = amount / 2;

		// Get current right movement (no left vector) 
		FVector right = SwordStance::avatarPtr->GetActorRightVector();

		// Add amount to movement
		SwordStance::avatarPtr->AddMovementInput(right, amount);
	}
}

void BlockSwordStance::MoveLeft(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (SwordStance::avatarPtr->pController && amount) {

		// Half the amount when moving
		amount = amount / 2;

		// Get current right movemnet
		FVector right = SwordStance::avatarPtr->GetActorRightVector();

		// Add amount to movmenet, in this case subract since it is left
		SwordStance::avatarPtr->AddMovementInput(right, -amount);
	}
}


void BlockSwordStance::Yaw(float amount) {

	// Change sword position based on mouse position
	//avatarPtr->swordFocalPoint->update(avatarPtr->pController);

	// If sword stance is activated (can turn camera)
	if (canTurnCamera) {

		avatarPtr->bUseControllerRotationYaw = true;
		avatarPtr->avatarMovementComponent->bOrientRotationToMovement = false;
		
		// Dont enter the body of this function if the controller is not set up, or amount == 0; 
		if (avatarPtr->pController && amount) {
			avatarPtr->AddControllerYawInput(avatarPtr->getBaseYawTurnSpeed() * amount * avatarPtr->GetWorld()->GetDeltaSeconds());
		}
	}
}

void BlockSwordStance::Pitch(float amount) {
	// Change sword position based on mouse position
	//avatarPtr->swordFocalPoint->update(avatarPtr->pController);

	if (canTurnCamera) {

		// Dont enter the body of this function if the controller is not set up, or amount == 0; 
		if (avatarPtr->pController && amount) {
			avatarPtr->AddControllerPitchInput(avatarPtr->getBasePitchTurnSpeed() * amount * avatarPtr->GetWorld()->GetDeltaSeconds());
		}
	}
}

void BlockSwordStance::swordStanceActivation() {

	// For block stance, activating the sword stance allows the user to turn their camera while blocking
	canTurnCamera = true; 

	// Turn off ability to change focal point since turniung camera
	applyRotationToSwordFocalPoint = false;
}

void BlockSwordStance::swordStanceDeactivation() {

	// Turn off ability to turn camera 
	canTurnCamera = false;
	applyRotationToSwordFocalPoint = true;
}