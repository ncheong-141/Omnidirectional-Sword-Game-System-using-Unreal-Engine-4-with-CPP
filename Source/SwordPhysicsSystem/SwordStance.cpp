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
	}
}

void SwordStance::MoveBack(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0
	if (avatarPtr->pController && amount) {

		// Get the value of the forward vector
		FVector avatarForwardVector = avatarPtr->GetActorForwardVector();

		// Subtract movement input to the avatar forward vector. 
		avatarPtr->AddMovementInput(avatarForwardVector, -amount);
	}
}

void SwordStance::MoveRight(float amount) {

	// Dont enter the body ofthis function if the controller is not set up, or amount == 0; 
	if (avatarPtr->Controller && amount) {

		// Get current right movement (no left vector) 
		FVector avatarRightVector = avatarPtr->GetActorRightVector();

		avatarPtr->AddMovementInput(avatarRightVector, amount);
	}
}

void SwordStance::MoveLeft(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->pController && amount) {

		// Get current right movement (no left vector) 
		FVector avatarRightVector = avatarPtr->GetActorRightVector();

		avatarPtr->AddMovementInput(avatarRightVector, -amount);
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

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->pController) {

		// Set WASD control to false 
		

		// Dodge action mappings (should be only one)
		TArray<FInputActionKeyMapping> dodgeActionMappings;
		avatarPtr->pController->inputSettings->GetActionMappingByName("Dodge", dodgeActionMappings);

		// Determine which WASD is pressed and if dodge key is active 
		// Current keys pressed maximum size of 2 since custom controller does not allow for more than 1 WASD input to be stored
		
		// Temp key containers (could optimise this and store the dodge key in the controller for reference)
		FKey WASDkeyPressed = FKey(FName(TEXT("None")));
		FKey dodgeKey = dodgeActionMappings[dodgeActionMappings.Num() - 1].Key; // Note, since the dodge key is specified first in the input UI section, this is its position

		// Boolean for switching 
		bool dodgeKeyActive = false; 

		// check for dodge key 
		for (int i = 0; i < avatarPtr->pController->getCurrentKeysPressed().Num(); i++) {
			
			// Check if it = dodge key, if not then set
			if (avatarPtr->pController->getCurrentKeysPressed()[i].key != dodgeKey) {
				WASDkeyPressed = avatarPtr->pController->getCurrentKeysPressed()[i].key;
			}
			// Check for dodge key
			else if (avatarPtr->pController->getCurrentKeysPressed()[i].key == dodgeKey) {
				dodgeKeyActive = true;
			}
		}

		// Debug
		if (dodgeKeyActive) {
			output.toHUD(FString("Dodge key active"), 2.f, false);
		}

		if (WASDkeyPressed.GetFName() != FName(TEXT("None"))) {
			output.toHUD(WASDkeyPressed.GetFName().ToString(), 2.f, false);
		}


		// If the dodge key is pressed and WASDkey is not
		if (dodgeKeyActive == true && WASDkeyPressed.GetFName() != FName(TEXT("None"))) {

			// Flow control for each WASD key 
			// (should change to switch statement to minimise latency)
			if (WASDkeyPressed == EKeys::A) {

				output.toHUD(FString("Dodge left"), 2.f, false);
			}
			else if (WASDkeyPressed == EKeys::D) {

				output.toHUD(FString("Dodge Right"), 2.f, false);
			}
			else if (WASDkeyPressed == EKeys::W) {

				output.toHUD(FString("Dodge Forward"), 2.f, false);
			}
			else if (WASDkeyPressed == EKeys::S) {

				output.toHUD(FString("Dodge Back"), 2.f, false);
			}
		}


		

		// Get animation curve of dodge

	}
}

