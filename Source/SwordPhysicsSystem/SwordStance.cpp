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
SwordStance::SwordStance(AAvatar* avatar, int stance_ID){
	
	// Set avatar pointer/State pattern context
	avatarPtr = avatar; 

	// Set Pointers/references to some commonly used Avatar objects
	//avatarForwardVector = &avatarPtr->GetActorForwardVector();
	//avatarRightVector = &avatarPtr->GetActorRightVector();

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
	if (avatarPtr->Controller && amount) {
	
		// Get the value of the forward vector
		FVector avatarForwardVector = avatarPtr->GetActorForwardVector(); 

		// Add movement input to the avatar forward vector. 
		avatarPtr->AddMovementInput(avatarForwardVector, amount);

		// Add movement input to the avatar forward vector. 
		// avatarPtr->AddMovementInput(*(avatarForwardVector), amount);
	}
}

void SwordStance::MoveBack(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0
	if (avatarPtr->Controller && amount) {

		// Get the value of the forward vector
		FVector avatarForwardVector = avatarPtr->GetActorForwardVector();

		// Subtract movement input to the avatar forward vector. 
		avatarPtr->AddMovementInput(avatarForwardVector, -amount);

		// Add amount to movement, since it is back it is subtract
		// avatarPtr->AddMovementInput(*(avatarForwardVector), -amount);
	}
}

void SwordStance::MoveRight(float amount) {

	// Dont enter the body ofthis function if the controller is not set up, or amount == 0; 
	if (avatarPtr->Controller && amount) {

		// Get current right movement (no left vector) 
		FVector avatarRightVector = avatarPtr->GetActorRightVector();

		avatarPtr->AddMovementInput(avatarRightVector, amount);

		// Add amount to movement
		// avatarPtr->AddMovementInput(*(avatarRightVector), amount);
	}
}

void SwordStance::MoveLeft(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->Controller && amount) {

		// Get current right movement (no left vector) 
		FVector avatarRightVector = avatarPtr->GetActorRightVector();

		avatarPtr->AddMovementInput(avatarRightVector, -amount);

		// Add amount to movmenet, in this case subract since it is left
		// avatarPtr->AddMovementInput(*(avatarRightVector), -amount);
	}
}


void SwordStance::Yaw(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->Controller && amount) {

		// Here 200 is mouse sensitivity (hardcoded for this case), getworld...etc gives you the amount of time that passed between the last frame and this frame
		avatarPtr->AddControllerYawInput(avatarPtr->baseYawTurnSpeed * amount * avatarPtr->GetWorld()->GetDeltaSeconds());
	}
}

void SwordStance::Pitch(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->Controller && amount) {

		// Here 200 is mouse sensitivity (hardcoded for this case), getworld...etc gives you the amount of time that passed between the last frame and this frame
		avatarPtr->AddControllerPitchInput(avatarPtr->basePitchTurnSpeed * amount * avatarPtr->GetWorld()->GetDeltaSeconds());
	}
}


/* Action functions */

void SwordStance::jump() {

	// Use inbuilt UE function for characters (can set characteristics in cosntructor)
	avatarPtr->Jump();
}
