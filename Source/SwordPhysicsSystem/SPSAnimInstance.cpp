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
	forwardMovementCurveCurrentValue = 0; 
	forwardMovementCurveLastFrameValue = 0; 

	rightMovementCurveCurrentValue = 0;
	upMovementCurveCurrentValue = 0; 
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
		forwardMovementCurveLastFrameValue = forwardMovementCurveCurrentValue;
		forwardMovementCurveCurrentValue = this->GetCurveValue(FName("ForwardMovement"));
		rightMovementCurveCurrentValue = this->GetCurveValue(FName("RightMovement"));
		upMovementCurveCurrentValue = this->GetCurveValue(FName("UpMovement"));

		UE_LOG(LogTemp, Display, TEXT("Forward movement value: %f"), forwardMovementCurveCurrentValue);
		UE_LOG(LogTemp, Display, TEXT("Right movement value: %f"), rightMovementCurveCurrentValue);
		UE_LOG(LogTemp, Display, TEXT("Up movement value: %f"), upMovementCurveCurrentValue);
	
		// Apply movemnets 

		if (animatedAvatar->isInDodge) {
			FVector fwdVec = animatedAvatar->GetActorForwardVector();
			UE_LOG(LogTemp, Display, TEXT("FV X: %f, FV Y: %f, FV Z: %f"), fwdVec.X, fwdVec.Y, fwdVec.Z);

			// Get current speed
			float currentForwardVel = animatedAvatar->inputVelocity_X * animatedAvatar->GetCharacterMovement()->GetMaxSpeed();
			
			// distance due to current velocity and acceleration between this and alst frame
			float fwdVecVelScale = (currentForwardVel + ((forwardMovementCurveCurrentValue - forwardMovementCurveLastFrameValue) * DeltaSeconds));
			float fwdVecDistScale = (currentForwardVel*DeltaSeconds) + (0.5f* ((forwardMovementCurveCurrentValue - forwardMovementCurveLastFrameValue)/DeltaSeconds) * DeltaSeconds * DeltaSeconds);

			UE_LOG(LogTemp, Display, TEXT("fwdVecVelScale: %f"), fwdVecVelScale);
			UE_LOG(LogTemp, Display, TEXT("fwdVecDistScale: %f"), fwdVecDistScale);

			fwdVec = fwdVec * fwdVecDistScale;
			UE_LOG(LogTemp, Display, TEXT("FV X: %f, FV Y: %f, FV Z: %f"), fwdVec.X, fwdVec.Y, fwdVec.Z);

			// Get current actor location 
			FVector currentLocation = animatedAvatar->GetActorLocation();
			UE_LOG(LogTemp, Display, TEXT("CLV X: %f, CLV Y: %f, CLV Z: %f"), currentLocation.X, currentLocation.Y, currentLocation.Z);

			// Calculate new location
			FVector newLocation = currentLocation + fwdVec;
			UE_LOG(LogTemp, Display, TEXT("NLV X: %f, NLV Y: %f, NLV Z: %f"), newLocation.X, newLocation.Y, newLocation.Z);


			// Set the location
			animatedAvatar->SetActorLocation(newLocation);
		}
	}
}
