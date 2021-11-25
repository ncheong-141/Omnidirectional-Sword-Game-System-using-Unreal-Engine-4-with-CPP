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
	fMovementDistanceCurveCurrentValue = 0.f;
	fMovementDistanceCurveLastFrameValue = 0.f;

	rMovementDistanceCurveCurrentValue = 0.f;
	rMovementDistanceCurveLastFrameValue = 0.f;

	upMovementDistanceCurveCurrentValue = 0.f;
	upMovementDistanceCurveLastFrameValue = 0.f;

	//righthandMovementCurveCUrrentFrameValue = 0.f;

	resetReadingOfAnimationCurve = false;
	animationCurrentlyPlaying = false;
}

USPSAnimInstance::~USPSAnimInstance() {

}


void USPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {

	// Call super function of this function 
	UAnimInstance::NativeUpdateAnimation(DeltaSeconds);
	
	// Check if actor exists so to not crash if not
	if (animatedAvatar != nullptr) {


		/* Reading of curve data */
		
		// Reset the distnace readings when appropiate (required for animation transitions)
		//UE_LOG(LogTemp, Display, TEXT("resetReadingOfAnimationCurve: %d"), resetReadingOfAnimationCurve);
		if (resetReadingOfAnimationCurve) {

			// Reset current and last frame curve calues
			fMovementDistanceCurveLastFrameValue = 0;
			fMovementDistanceCurveCurrentValue = 0;

			rMovementDistanceCurveLastFrameValue = 0;
			rMovementDistanceCurveCurrentValue = 0;

			upMovementDistanceCurveLastFrameValue = 0;
			upMovementDistanceCurveCurrentValue = 0;

			// Toggle reset switch
			resetReadingOfAnimationCurve = false;
			UE_LOG(LogTemp, Display, TEXT("resetReadingOfAnimationCurve: %d"), resetReadingOfAnimationCurve);
		}

		// If there is an animation currently playing
		if (animationCurrentlyPlaying) {

			// Calculate the current time
			if (currentTime + DeltaSeconds < totalAnimationDuration) {
				currentTime += DeltaSeconds;
				//UE_LOG(LogTemp, Display, TEXT("Current time: %f"), currentTime);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Going over total time"));
			}

			// Set curve current and last frame values
			
			// ForwardMovement distance
			fMovementDistanceCurveLastFrameValue = fMovementDistanceCurveCurrentValue;
			
			if (ForwardMovementDistanceFloatCurve != nullptr) {
				fMovementDistanceCurveCurrentValue = ForwardMovementDistanceFloatCurve->Evaluate(currentTime);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("ForwardMovement curve nullptr in %s"), __FUNCTION__);
			}

			// Right movement distance
			rMovementDistanceCurveLastFrameValue = rMovementDistanceCurveCurrentValue;
			
			if (RightMovementDistanceFloatCurve != nullptr) {
				rMovementDistanceCurveCurrentValue = RightMovementDistanceFloatCurve->Evaluate(currentTime);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("RightMovement curve nullptr in %s"), __FUNCTION__);
			}

			// Up movement distance
			upMovementDistanceCurveLastFrameValue = upMovementDistanceCurveCurrentValue;
			
			if (UpMovementDistanceFloatCurve != nullptr) {
				upMovementDistanceCurveCurrentValue = UpMovementDistanceFloatCurve->Evaluate(currentTime);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("UpdMovement curve nullptr in %s"), __FUNCTION__);
			}
		}


		// Debug
		UE_LOG(LogTemp, Display, TEXT("CanDamage: %d"), animatedAvatar->MeleeWeapon->canDamage);
		UE_LOG(LogTemp, Display, TEXT("IsInAttackMotion: %d"), animatedAvatar->isInAttackMotion);

		/* Apply animation curve values */
		// The curve current values are updated in the animation notification states
		// Animations are applied in this class as it makes sense to apply the animation movement per animation tick
		if (animatedAvatar->isInDodge) {

			// Apply animation curve movement
			animatedAvatar->applyAnimMovement(this);
		}

		if (animatedAvatar->isInAttackMotion) {

			// Apply animation curve movement
			animatedAvatar->applyAnimMovement(this);

			// Set the right hand speed 

		}
	}
}


void USPSAnimInstance::setMovementFloatCurvePointers(UAnimSequenceBase* Animation) {

	if (Animation != nullptr) {
		// Get the curve data reference from the animation
		const FRawCurveTracks& curves = Animation->GetCurveData();

		// Establish the variable for curve name
		FSmartName curveName;

		// Get data
		Animation->GetSkeleton()->GetSmartNameByName(USkeleton::AnimCurveMappingName, TEXT("ForwardMovement"), curveName);
		ForwardMovementDistanceFloatCurve = static_cast<const FFloatCurve*>(curves.GetCurveData(curveName.UID));

		Animation->GetSkeleton()->GetSmartNameByName(USkeleton::AnimCurveMappingName, TEXT("RightMovement"), curveName);
		RightMovementDistanceFloatCurve = static_cast<const FFloatCurve*>(curves.GetCurveData(curveName.UID));

		Animation->GetSkeleton()->GetSmartNameByName(USkeleton::AnimCurveMappingName, TEXT("UpMovement"), curveName);
		UpMovementDistanceFloatCurve = static_cast<const FFloatCurve*>(curves.GetCurveData(curveName.UID));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Animation is nullptr in %s"), __FUNCTION__);
	}

}

void USPSAnimInstance::setAttackFloatCurvePointers(UAnimSequenceBase* Animation) {

	if (Animation != nullptr) {
		// Get the curve data reference from the animation
		const FRawCurveTracks& curves = Animation->GetCurveData();

		// Establish the variable for curve name
		FSmartName curveName;

		// Get data for right hand movement speed
		Animation->GetSkeleton()->GetSmartNameByName(USkeleton::AnimCurveMappingName, TEXT("thumb_01_r_MovementSpeed"), curveName);
		RightHandMovementSpeedFloatCurve = static_cast<const FFloatCurve*>(curves.GetCurveData(curveName.UID));

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Animation is nullptr in %s"), __FUNCTION__);
	}
}