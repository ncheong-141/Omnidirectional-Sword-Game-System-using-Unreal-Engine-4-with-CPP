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


		// Determine the dodge key from the player controller
		// Dodge action mappings (should be WASD and Lshift) (Should move the dodge action mapping to the SPSPlayer controller)
		FKey dodgeKey = avatarPtr->pController->dodgeActionMappings[avatarPtr->pController->dodgeActionMappings.Num() - 1].Key; // Note, since the dodge key is specified first in the input UI section, this is its position


		if (avatarPtr->pController->IsInputKeyDown(dodgeKey)) {

			// Flow control for each WASD key 
			// (should change to switch statement to minimise latency)
			if (avatarPtr->pController->IsInputKeyDown(EKeys::A)) {

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Dodge Left"));
				UE_LOG(LogTemp, Display, TEXT("Dodge Left"))
				avatarPtr->isInDodge = true;
				avatarPtr->dodgeDirection = 2;
			}
			else if (avatarPtr->pController->IsInputKeyDown(EKeys::D)) {

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Dodge Right"));
				UE_LOG(LogTemp, Display, TEXT("Dodge Right"))
				avatarPtr->isInDodge = true;
				avatarPtr->dodgeDirection = 3;
			}
			else if (avatarPtr->pController->IsInputKeyDown(EKeys::W)) {

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Dodge Forward"));
				UE_LOG(LogTemp, Display, TEXT("Dodge Forward"))
				avatarPtr->isInDodge = true;
				avatarPtr->dodgeDirection = 0;

				applyAnimationCurveCardinalMovement(); 
			}
			else if (avatarPtr->pController->IsInputKeyDown(EKeys::S)) {

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Dodge Back"));
				UE_LOG(LogTemp, Display, TEXT("Dodge Back"))
				avatarPtr->isInDodge = true;
				avatarPtr->dodgeDirection = 1;
			}
		}

		// Get animation curve of dodge
	}
}



/* Internal helper functions for all sword stance classes */
void SwordStance::applyAnimationCurveCardinalMovement() {

	// Get animation instance of the avatar (SPSAnimInstance)
	UAnimInstance* avatarAnimInstance = avatarPtr->GetMesh()->GetAnimInstance(); 

	// Get the Animation montage (blended animation sequences currently playing)
	UAnimMontage* currentAnimMontage = avatarAnimInstance->GetCurrentActiveMontage();

	if (currentAnimMontage == nullptr) {

		UE_LOG(LogTemp, Display, TEXT("Shit"));
		return;
	}

	// Get curve smartNames and therefore ID to extract data from
	//FSmartName avatarAnimForwardMovementCurve;
	//FSmartName avatarAnimRightMovementCurve; 
	//FSmartName avatarAnimUpMovementCurve;
	//currentAnimMontage->GetSkeleton()->GetSmartNameByName(USkeleton::AnimCurveMappingName, TEXT("ForwardMovement"), avatarAnimForwardMovementCurve);
	//currentAnimMontage->GetSkeleton()->GetSmartNameByName(USkeleton::AnimCurveMappingName, TEXT("RightMovement"), avatarAnimRightMovementCurve);
	//currentAnimMontage->GetSkeleton()->GetSmartNameByName(USkeleton::AnimCurveMappingName, TEXT("UpMovement"), avatarAnimUpMovementCurve);

	//UE_LOG(LogTemp, Display, TEXT("Curve name: %s"), *(avatarAnimForwardMovementCurve.DisplayName.ToString()));
	//UE_LOG(LogTemp, Display, TEXT("Curve name: %s"), *(avatarAnimRightMovementCurve.DisplayName.ToString()));
	//UE_LOG(LogTemp, Display, TEXT("Curve name: %s"), *(avatarAnimUpMovementCurve.DisplayName.ToString()));

	// Get the curve data at corresponding curve ID
	auto avatarAnimCurves = currentAnimMontage->GetCurveData().GetCurveData(0);
	UE_LOG(LogTemp, Display, TEXT("Curve name: %s"), *(avatarAnimCurves->Name.DisplayName.ToString()));

	//// Get the corresponding float curve 
	//TArray<const FFloatCurve*> avatarAnimFloatCurves;
	//avatarAnimFloatCurves.Add(static_cast<const FFloatCurve*>(avatarAnimCurves.GetCurveData(avatarAnimForwardMovementCurve.UID)));
	//avatarAnimFloatCurves.Add(static_cast<const FFloatCurve*>(avatarAnimCurves.GetCurveData(avatarAnimRightMovementCurve.UID)));
	//avatarAnimFloatCurves.Add(static_cast<const FFloatCurve*>(avatarAnimCurves.GetCurveData(avatarAnimUpMovementCurve.UID)));

	//// Debug
	//float minTime;
	//float maxTime; 
	//avatarAnimFloatCurves[0]->FloatCurve.GetTimeRange(minTime, maxTime);

	//UE_LOG(LogTemp, Display, TEXT("Min time: %f"), minTime);
	//UE_LOG(LogTemp, Display, TEXT("Max time: %f"), maxTime);

	//float currentTime = minTime; 

	//while (currentTime < maxTime) {

	//	// Get data from curvve
	//	float data = avatarAnimFloatCurves[0]->Evaluate(currentTime); 
	//	UE_LOG(LogTemp, Display, TEXT("Time: %f"), currentTime);
	//	UE_LOG(LogTemp, Display, TEXT("ForwardMovement: %f"), data);

	//	// add to the current time
	//	currentTime = currentTime + avatarPtr->GetWorld()->GetDeltaSeconds(); 
	//}



}
