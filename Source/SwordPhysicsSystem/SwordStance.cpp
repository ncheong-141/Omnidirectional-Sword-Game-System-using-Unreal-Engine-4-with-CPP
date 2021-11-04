// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordStance.h"

// Game class files
#include "Avatar.h"
#include "SPSPlayerController.h"

// Debug file
#include "DebugOutput.h"

// UE4 Global functions
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"



/* Internal function implementation */

// Check if the dodge key was last pressed function for all movement functions
bool dodgeKeyCurrentlyPressed(AAvatar* avatarPtr) {

	// Check if player controller set
	if (avatarPtr->pController) {

		// Get current key pressed from custom player controller and compared with dodge key
		if (*(avatarPtr->pController->getCurrentKeyPressed()) == EKeys::LeftShift) {
			return true;
		}
	}
	return false;
}


// Constructor and destructor implementation
SwordStance::SwordStance(AAvatar* avatar, int stance_ID){
	
	// Set avatar pointer/State pattern context
	avatarPtr = avatar; 

	// Set the stance ID
	stanceID = stance_ID; 
}

SwordStance::SwordStance(){

	// Set avatar pointer to NULL
	avatarPtr = NULL;

}

SwordStance::~SwordStance() {
}


// Virtual functions likely common to all states but can be overidden when required
void SwordStance::displayStance() {
	DebugOutput output = DebugOutput();

	output.toHUD(FString("AHH"), 2.f, false);
}

// Note, input of amount is 0 or 1
void SwordStance::MoveForward(float amount) {

	// Don't enter the body of this function if the controller is not set up, or amount == 0
	if (avatarPtr->pController && amount) {
	
		// Get the value of the forward vector
		FVector avatarForwardVector = avatarPtr->GetActorForwardVector(); 

		// Add movement input to the avatar forward vector. 
		avatarPtr->AddMovementInput(avatarForwardVector, amount);

		// Check if dodge button is now activated
		if (dodgeKeyCurrentlyPressed(avatarPtr)) {
			SwordStance::dodge();
		}
	}
}

void SwordStance::MoveBack(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0
	if (avatarPtr->pController && amount) {

		// Get the value of the forward vector
		FVector avatarForwardVector = avatarPtr->GetActorForwardVector();

		// Subtract movement input to the avatar forward vector. 
		avatarPtr->AddMovementInput(avatarForwardVector, -amount);

		// Check if dodge button is now activated
		if (dodgeKeyCurrentlyPressed(avatarPtr)) {
			SwordStance::dodge();
		}
	}
}

void SwordStance::MoveRight(float amount) {

	// Dont enter the body ofthis function if the controller is not set up, or amount == 0; 
	if (avatarPtr->Controller && amount) {

		// Get current right movement (no left vector) 
		FVector avatarRightVector = avatarPtr->GetActorRightVector();

		avatarPtr->AddMovementInput(avatarRightVector, amount);

		// Check if dodge button is now activated
		if (dodgeKeyCurrentlyPressed(avatarPtr)) {
			SwordStance::dodge();
		}
	}
}

void SwordStance::MoveLeft(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->pController && amount) {

		// Get current right movement (no left vector) 
		FVector avatarRightVector = avatarPtr->GetActorRightVector();

		avatarPtr->AddMovementInput(avatarRightVector, -amount);

		// Check if dodge button is now activated
		if (dodgeKeyCurrentlyPressed(avatarPtr)) {
			SwordStance::dodge();
		}
	}
}


void SwordStance::Yaw(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->pController && amount) {

		// Here 200 is mouse sensitivity (hardcoded for this case), getworld...etc gives you the amount of time that passed between the last frame and this frame
		avatarPtr->AddControllerYawInput(avatarPtr->baseYawTurnSpeed * amount * avatarPtr->GetWorld()->GetDeltaSeconds());
	}
}

void SwordStance::Pitch(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->pController && amount) {

		// Here 200 is mouse sensitivity (hardcoded for this case), getworld...etc gives you the amount of time that passed between the last frame and this frame
		avatarPtr->AddControllerPitchInput(avatarPtr->basePitchTurnSpeed * amount * avatarPtr->GetWorld()->GetDeltaSeconds());
	}
}


/* Action functions */

void SwordStance::jump() {

	// Use inbuilt UE function for characters (can set characteristics in cosntructor)
	avatarPtr->Jump();
}

void SwordStance::dodge() {

	DebugOutput output = DebugOutput();

	output.toHUD(FString("In Dodge"), 2.f, false);

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->pController) {

		// Set WASD control to false 

		// Determine if its a WASD dodge (should change to switch statement to minimise latency)
		// Need to find out how to get key value
		if (avatarPtr->pController->IsInputKeyDown(EKeys::A)) {

			output.toHUD(FString("Dodge left"), 2.f, false);
		}
		else if (avatarPtr->pController->IsInputKeyDown(EKeys::D)) {

			output.toHUD(FString("Dodge Right"), 2.f, false);

		}
		else if (avatarPtr->pController->IsInputKeyDown(EKeys::W)) {

			output.toHUD(FString("Dodge Forward"), 2.f, false);

		}
		else if (avatarPtr->pController->IsInputKeyDown(EKeys::S)) {

			output.toHUD(FString("Dodge Back"), 2.f, false);

		}
		


		
		
		// Get animation curve of dodge

	}

}

