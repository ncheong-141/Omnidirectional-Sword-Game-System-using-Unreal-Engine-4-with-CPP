// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordStance.h"

// Game class files
#include "Avatar.h"

// Debug file
#include "DebugOutput.h"

// UE4 Global functions
#include "Kismet/GameplayStatics.h"

//#include "Engine/GameFramework/UCharacterMovementComponent.h"


// Constructor and destructor implementation
SwordStance::SwordStance(AAvatar* avatar){
	
	// Set avatar pointer/State pattern context
	avatarPtr = avatar; 

	// Set avatar playstates to default to untrue
	cardinalMovementLock = false;
}

SwordStance::~SwordStance(){

	// Set avatar pointer to NULL
	avatarPtr = NULL;

	// Set avatar playstates to default to untrue
	cardinalMovementLock = false;
}


// Virtual functions likely common to all states but can be overidden when required
void SwordStance::displayStance() {
	DebugOutput output = DebugOutput();

	output.toHUD(FString("AHH"), 2.f, false);
}

// Note, input of amount is 0 or 1
void SwordStance::MoveForward(float amount) {

	// Check if cardinal movement is disabled (e.g. during a certain action)
	if (cardinalMovementLock) {
		return;
	}

	// Don't enter the body of this function if the controller is not set up, or amount == 0
	if (avatarPtr->Controller && amount) {

		// Get current forward movement
		FVector fwd = avatarPtr->GetActorForwardVector();

		// We call add movement to actually move the player by amount in the fwd direction
		avatarPtr->AddMovementInput(fwd, amount);
	}
}

void SwordStance::MoveBack(float amount) {

	// Check if cardinal movement is disabled (e.g. during a certain action)
	if (cardinalMovementLock) {
		return;
	}

	// Dont enter the body of this function if the controller is not set up, or amount == 0
	if (avatarPtr->Controller && amount) {

		// Get current forward movement (no back vector) 
		FVector fwd = avatarPtr->GetActorForwardVector();

		// Add amount to movement, since it is back it is subtract
		avatarPtr->AddMovementInput(fwd, -amount);
	}
}

void SwordStance::MoveRight(float amount) {

	// Check if cardinal movement is disabled (e.g. during a certain action)
	if (cardinalMovementLock) {
		return;
	}

	// Dont enter the body ofthis function if the controller is not set up, or amount == 0; 
	if (avatarPtr->Controller && amount) {

		// Get current right movement (no left vector) 
		FVector right = avatarPtr->GetActorRightVector();

		// Add amount to movement
		avatarPtr->AddMovementInput(right, amount);
	}
}

void SwordStance::MoveLeft(float amount) {

	// Check if cardinal movement is disabled (e.g. during a certain action)
	if (cardinalMovementLock) {
		return;
	}

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->Controller && amount) {

		// Get current right movemnet
		FVector right = avatarPtr->GetActorRightVector();

		// Add amount to movmenet, in this case subract since it is left
		avatarPtr->AddMovementInput(right, -amount);
	}
}


void SwordStance::Yaw(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->Controller && amount) {

		// Here 200 is mouse sensitivity (hardcoded for this case), getworld...etc gives you the amount of time that passed between the last frame and this frame
		avatarPtr->AddControllerYawInput(200.f * amount * avatarPtr->GetWorld()->GetDeltaSeconds());
	}
}

void SwordStance::Pitch(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->Controller && amount) {

		// Here 200 is mouse sensitivity (hardcoded for this case), getworld...etc gives you the amount of time that passed between the last frame and this frame
		avatarPtr->AddControllerPitchInput(200.f * amount * avatarPtr->GetWorld()->GetDeltaSeconds());
	}
}


/* Action functions */

void SwordStance::dodgeLeft(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->Controller) {

		// Lock cardinal movement 
		cardinalMovementLock = true; 

		// Get avatar vector and set X,Y components to 0 (as going to "overwrite" movement with dodge)
		FVector right = avatarPtr->GetActorRightVector();
		//right.X = 0.f;
		//right.Y = 0.f;

		/* Start the dodge */
		float dodgingTime = dodgeTime;

		UCharacterMovementComponent* move = avatarPtr->GetCharacterMovement();

		// Loop until dodging time is not over
		while (dodgingTime > 0) {

			// Add amount to movmenet, in this case subract since it is left
			avatarPtr->AddMovementInput(right, -100*amount);

			// Subtract the time between last frame and current (the time passed basically)
			dodgingTime = dodgingTime - 0.001 - avatarPtr->GetWorld()->GetDeltaSeconds();
		}

		// Unlock cardinal movement
		cardinalMovementLock = false;
	}

}