// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordStance.h"

// Game class files
#include "Avatar.h"
#include "SPSPlayerController.h"
#include "SPSAnimInstance.h"

// Debug file
#include "DebugOutput.h"

// UE4 Global functions
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"


SwordStance::SwordStance() {

}

// Constructor and destructor implementation
SwordStance::SwordStance(AAvatar* avatar, const int stance_ID) : stanceID(stance_ID){
	
	// Set avatar pointer/State pattern context
	avatarPtr = avatar; 
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
	if (avatarPtr->pController && amount) {

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
		avatarPtr->AddControllerYawInput(avatarPtr->getBaseYawTurnSpeed() * amount * avatarPtr->GetWorld()->GetDeltaSeconds());

	}
}

void SwordStance::Pitch(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->pController && amount) {

		// Here 200 is mouse sensitivity (hardcoded for this case), getworld...etc gives you the amount of time that passed between the last frame and this frame
		avatarPtr->AddControllerPitchInput(avatarPtr->getBasePitchTurnSpeed() * amount * avatarPtr->GetWorld()->GetDeltaSeconds());

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

		// Determine the dodge key from the player controller
		// Dodge action mappings (should be WASD and Lshift) (Should move the dodge action mapping to the SPSPlayer controller)
		FKey dodgeKey = avatarPtr->pController->dodgeActionMappings[avatarPtr->pController->dodgeActionMappings.Num() - 1].Key; // Note, since the dodge key is specified first in the input UI section, this is its position

		if (avatarPtr->pController->IsInputKeyDown(dodgeKey)) {

			// Flow control for each WASD key 
			// (should change to switch statement to minimise latency)
			if (avatarPtr->pController->IsInputKeyDown(EKeys::A)) {

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Dodge Left"));
				UE_LOG(LogTemp, Display, TEXT("Dodge Left"))
				avatarPtr->setAvatarIsInDodge(true);
				avatarPtr->setDodgeDirection(2);
			}
			else if (avatarPtr->pController->IsInputKeyDown(EKeys::D)) {

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Dodge Right"));
				UE_LOG(LogTemp, Display, TEXT("Dodge Right"))
				avatarPtr->setAvatarIsInDodge(true);
				avatarPtr->setDodgeDirection(3);
			}
			else if (avatarPtr->pController->IsInputKeyDown(EKeys::W)) {

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Dodge Forward"));
				UE_LOG(LogTemp, Display, TEXT("Dodge Forward"))
				avatarPtr->setAvatarIsInDodge(true);
				avatarPtr->setDodgeDirection(0);
			}
			else if (avatarPtr->pController->IsInputKeyDown(EKeys::S)) {

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Dodge Back"));
				UE_LOG(LogTemp, Display, TEXT("Dodge Back"))
				avatarPtr->setAvatarIsInDodge(true);
				avatarPtr->setDodgeDirection(1);
			}
		}
	}
}


void SwordStance::calculateAllowableSwordDirections() {

	// By default, do not change what is allowable (they are all initially true)
}

// Getters and setters
const AllowableSwordDirectionInformation* SwordStance::getAllowableSwordDirections() {
	return &allowableSwordDirections;
}