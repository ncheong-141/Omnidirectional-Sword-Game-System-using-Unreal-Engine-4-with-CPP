// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockSwordStance.h"
#include "Avatar.h"

// Debug output file
#include "DebugOutput.h"

BlockSwordStance::~BlockSwordStance()
{
}


// Virtual function override class function implementation
void BlockSwordStance::displayStance() {

	DebugOutput output = DebugOutput();

	output.toHUD(FString("Block Stance"), 2.f, false);
}


/* Player input WASD */

void BlockSwordStance::MoveForward(float amount) {

	// Don't enter the body of this function if the controller is not set up, or amount == 0
	if (SwordStance::avatarPtr->Controller && amount) {

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
	if (SwordStance::avatarPtr->Controller && amount) {

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
	if (SwordStance::avatarPtr->Controller && amount) {

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
	if (SwordStance::avatarPtr->Controller && amount) {

		// Half the amount when moving
		amount = amount / 2;

		// Get current right movemnet
		FVector right = SwordStance::avatarPtr->GetActorRightVector();

		// Add amount to movmenet, in this case subract since it is left
		SwordStance::avatarPtr->AddMovementInput(right, -amount);
	}
}
