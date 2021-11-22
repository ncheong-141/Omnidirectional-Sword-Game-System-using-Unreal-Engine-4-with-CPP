// Fill out your copyright notice in the Description page of Project Settings.


#include "SPSAnimInstance.h"

// Game classes
#include "Avatar.h"

// Unreal engine files
#include "GameFramework/CharacterMovementComponent.h"



USPSAnimInstance::USPSAnimInstance() {
	
	// Instantiate avatar pointer (set in BP)
	animatedAvatar = nullptr;

	// Instantiate current curve values (extracted in native update animation function)
	fMovementDistanceCurveCurrentValue = 0;
	fMovementDistanceCurveLastFrameValue = 0;

	rMovementDistanceCurveCurrentValue = 0;
	rMovementDistanceCurveLastFrameValue = 0;

	upMovementDistanceCurveCurrentValue = 0;
	upMovementDistanceCurveLastFrameValue = 0;

	allowReadingOfAnimationCurve = false;
	resetReadingOfAnimationCurve = false;
}

USPSAnimInstance::~USPSAnimInstance() {

}


void USPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {

	// Call super function of this function 
	UAnimInstance::NativeUpdateAnimation(DeltaSeconds);
	
	// Check if actor exists so to not crash if not
	if (animatedAvatar != nullptr) {

		if (resetReadingOfAnimationCurve) {

			fMovementDistanceCurveLastFrameValue = 0;
			fMovementDistanceCurveCurrentValue = 0;

			rMovementDistanceCurveLastFrameValue = 0;
			rMovementDistanceCurveCurrentValue = 0;

			upMovementDistanceCurveLastFrameValue = 0;
			upMovementDistanceCurveCurrentValue = 0;

			resetReadingOfAnimationCurve = false;
		}

		if (allowReadingOfAnimationCurve) {

			// Get current curve values
			fMovementDistanceCurveLastFrameValue = fMovementDistanceCurveCurrentValue;
			fMovementDistanceCurveCurrentValue = this->GetCurveValue(FName("ForwardMovement"));

			rMovementDistanceCurveLastFrameValue = rMovementDistanceCurveCurrentValue;
			rMovementDistanceCurveCurrentValue = this->GetCurveValue(FName("RightMovement"));

			upMovementDistanceCurveLastFrameValue = upMovementDistanceCurveCurrentValue;
			upMovementDistanceCurveCurrentValue = this->GetCurveValue(FName("UpMovement"));
		}

		// Debug
		FVector currentLocation = animatedAvatar->GetActorLocation();
		UE_LOG(LogTemp, Display, TEXT("resetReadingOfAnimationCurve: %d"), resetReadingOfAnimationCurve);
		UE_LOG(LogTemp, Display, TEXT("allowReadingOfAnimationCurve: %d"), allowReadingOfAnimationCurve);
		UE_LOG(LogTemp, Display, TEXT("CLV X: %f, CLV Y: %f, CLV Z: %f"), currentLocation.X, currentLocation.Y, currentLocation.Z);
		UE_LOG(LogTemp, Display, TEXT("Current forward movement in SPS anim: %f"), fMovementDistanceCurveCurrentValue)
		UE_LOG(LogTemp, Display, TEXT("Current right movement in SPS anim: %f"), rMovementDistanceCurveCurrentValue);


		// Apply animation curve values
		if (animatedAvatar->isInDodge) {
			animatedAvatar->applyAnimMovement_Dodge(this);
		}
	}
}
