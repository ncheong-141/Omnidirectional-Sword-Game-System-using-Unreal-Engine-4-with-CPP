// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordStance.h"

// Game class files
#include "Avatar.h"
#include "SPSPlayerController.h"
#include "SPSAnimInstance.h"
#include "AvatarHUD.h"

// UE4 Global functions
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

SwordStance::SwordStance() {

}

// Constructor and destructor implementation
SwordStance::SwordStance(AAvatar* avatar, const int stance_ID, bool applyRotationToSFP, bool lockOn_Target) : stanceID(stance_ID), 
																							applyRotationToSwordFocalPoint(applyRotationToSFP), 
																							stanceActivated(false), stanceActivationJustStarted(false), stanceActivationJustEnded(false),
																							lockOnTarget(lockOn_Target){
	
	// Set avatar pointer/State pattern context
	avatarPtr = avatar; 
}


SwordStance::~SwordStance() {
}


// Virtual functions likely common to all states but can be overidden when required
void SwordStance::displayStance() {
}

// Note, input of amount is 0 or 1
void SwordStance::MoveForward(float amount) {

	// Don't enter the body of this function if the controller is not set up, or amount == 0
	if (avatarPtr->pController && amount) {
	
		
		//FVector avatarForwardVector = avatarPtr->GetActorForwardVector(); 
		// Get the value of the camera direction vector	
		FVector cameraDirection = FRotationMatrix(avatarPtr->pController->GetControlRotation()).GetScaledAxis(EAxis::X);

		// Add movement input to the avatar forward vector. 
		avatarPtr->AddMovementInput(cameraDirection, amount);
	}
}

void SwordStance::MoveBack(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0
	if (avatarPtr->pController && amount) {

		// Get the value of the camera direction vector
		FVector cameraDirection = FRotationMatrix(avatarPtr->pController->GetControlRotation()).GetScaledAxis(EAxis::X);

		// Subtract movement input to the avatar forward vector. 
		avatarPtr->AddMovementInput(cameraDirection, -amount);
	}
}

void SwordStance::MoveRight(float amount) {

	// Dont enter the body ofthis function if the controller is not set up, or amount == 0; 
	if (avatarPtr->pController && amount) {

		// Get current right movement (no left vector) 
		//FVector avatarRightVector = avatarPtr->GetActorRightVector();
		
		// Get the value of the camera direction vector
		FVector cameraDirection = FRotationMatrix(avatarPtr->pController->GetControlRotation()).GetScaledAxis(EAxis::Y);

		avatarPtr->AddMovementInput(cameraDirection, amount);
	}
}

void SwordStance::MoveLeft(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->pController && amount) {

		// Get current right movement (no left vector) 
		//FVector avatarRightVector = avatarPtr->GetActorRightVector();

		// Get the value of the camera direction vector
		FVector cameraDirection = FRotationMatrix(avatarPtr->pController->GetControlRotation()).GetScaledAxis(EAxis::Y);

		avatarPtr->AddMovementInput(cameraDirection, -amount);
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
		avatarPtr->AddControllerPitchInput(-avatarPtr->getBasePitchTurnSpeed() * amount * avatarPtr->GetWorld()->GetDeltaSeconds());

	}
}


/* Action functions */

void SwordStance::jump() {

	// Use inbuilt UE function for characters (can set characteristics in cosntructor)
	avatarPtr->Jump();
}

void SwordStance::dodge() {


	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (avatarPtr->pController) {

		// Determine the dodge key from the player controller
		// Dodge action mappings (should be WASD and Lshift) (Should move the dodge action mapping to the SPSPlayer controller)
		FKey dodgeKey = avatarPtr->pController->dodgeActionMappings[avatarPtr->pController->dodgeActionMappings.Num() - 1].Key; // Note, since the dodge key is specified first in the input UI section, this is its position

		// Set avatar to follow with camera yaw
		avatarPtr->bUseControllerRotationYaw = true;
		avatarPtr->avatarMovementComponent->bOrientRotationToMovement = false;

		if (avatarPtr->pController->IsInputKeyDown(dodgeKey)) {

			// Flow control for each WASD key 
			// (should change to switch statement to minimise latency)
			if (avatarPtr->pController->IsInputKeyDown(EKeys::A)) {

				avatarPtr->setAvatarIsInDodge(true);
				avatarPtr->setDodgeDirection(2);
			}
			else if (avatarPtr->pController->IsInputKeyDown(EKeys::D)) {

				avatarPtr->setAvatarIsInDodge(true);
				avatarPtr->setDodgeDirection(3);
			}
			else if (avatarPtr->pController->IsInputKeyDown(EKeys::W)) {

				avatarPtr->setAvatarIsInDodge(true);
				avatarPtr->setDodgeDirection(0);
			}
			else if (avatarPtr->pController->IsInputKeyDown(EKeys::S)) {

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