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
}

USPSAnimInstance::~USPSAnimInstance() {

}


void USPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {

	// Call super function of this function 
	UAnimInstance::NativeUpdateAnimation(DeltaSeconds);
	
	//UE_LOG(LogTemp, Display, TEXT("Curve movement value: %f"), movementCurveCurrentValue);
	

	// Check if actor exists so to not crash if not
	if (animatedAvatar != nullptr) {

		// Get current curve values
		fMovementDistanceCurveLastFrameValue = fMovementDistanceCurveCurrentValue;
		fMovementDistanceCurveCurrentValue = this->GetCurveValue(FName("ForwardMovement"));
		
		rMovementDistanceCurveLastFrameValue = rMovementDistanceCurveCurrentValue;
		rMovementDistanceCurveCurrentValue = this->GetCurveValue(FName("RightMovement"));
		
		upMovementDistanceCurveLastFrameValue = upMovementDistanceCurveCurrentValue;
		upMovementDistanceCurveCurrentValue = this->GetCurveValue(FName("UpMovement"));

	
		// Apply movemnets 
		if (animatedAvatar->isInDodge) {
			animatedAvatar->applyAnimMovement_Dodge();
		}
	}
}
