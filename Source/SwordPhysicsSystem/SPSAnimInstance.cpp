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

	righthandMovementCurveCurrentValue = 0.f;
	animationCurrentlyPlaying = false;

	currentTime = 0.f;
	lastFrameTime = 0.f;
}

USPSAnimInstance::~USPSAnimInstance() {

}


void USPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {

	// Call super function of this function 
	UAnimInstance::NativeUpdateAnimation(DeltaSeconds);
	
	// Check if actor exists so to not crash if not
	if (animatedAvatar != nullptr) {

		// Check if stance just started (start of attack)
		if (animatedAvatar->getStance()->stanceActivationJustStarted) {

			// Set current time to zero
			currentTime = 0.f;

			// Unflag the flag
			animatedAvatar->getStance()->stanceActivationJustStarted = false;
		}

		// If there is an animation currently playing
		if (animationCurrentlyPlaying) {

			UE_LOG(LogTemp, Display, TEXT("Anim total dura time: %f"), totalNotificationDuration);
			if (currentlyPlayingAnimation) {
				UE_LOG(LogTemp, Display, TEXT("Anim total dura time from object from function: %f"), currentlyPlayingAnimation->GetPlayLength());
				UE_LOG(LogTemp, Display, TEXT("Anim total dura time from object from variable: %f"), currentlyPlayingAnimation->SequenceLength);
				UE_LOG(LogTemp, Display, TEXT("Anim RateScake from object from variable: %f"), currentlyPlayingAnimation->RateScale);
			}

			// Calculate the current time
			// Notificaiton duration scaled by rate scale
			if (currentTime + DeltaSeconds < totalNotificationDuration) {
				lastFrameTime = currentTime;
				currentTime += DeltaSeconds;
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("over total time"));

				// End attack? Reset time?
				// Reset time


			}
			//UE_LOG(LogTemp, Display, TEXT("Current time: %f"), currentTime);


			/* Reading of curve data */
			// Set curve current and last frame values
			// Note last frame value is obtained from the curve as the curves can switch depending on anim playing

			// ForwardMovement distance
			if (ForwardMovementDistanceFloatCurve != nullptr) {
				fMovementDistanceCurveLastFrameValue = ForwardMovementDistanceFloatCurve->Evaluate(lastFrameTime);;
				fMovementDistanceCurveCurrentValue = ForwardMovementDistanceFloatCurve->Evaluate(currentTime);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("ForwardMovement curve nullptr in %s"), __FUNCTION__);
			}

			// Right movement distance			
			if (RightMovementDistanceFloatCurve != nullptr) {
				rMovementDistanceCurveLastFrameValue = RightMovementDistanceFloatCurve->Evaluate(lastFrameTime);
				rMovementDistanceCurveCurrentValue = RightMovementDistanceFloatCurve->Evaluate(currentTime);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("RightMovement curve nullptr in %s"), __FUNCTION__);
			}

			// Up movement distance
			if (UpMovementDistanceFloatCurve != nullptr) {
				upMovementDistanceCurveLastFrameValue = UpMovementDistanceFloatCurve->Evaluate(lastFrameTime);;
				upMovementDistanceCurveCurrentValue = UpMovementDistanceFloatCurve->Evaluate(currentTime);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("UpdMovement curve nullptr in %s"), __FUNCTION__);
			}
		
			// Right hand movement speed
			if (RightHandMovementSpeedFloatCurve != nullptr) {
				righthandMovementCurveCurrentValue = RightHandMovementSpeedFloatCurve->Evaluate(currentTime);
			}

			/* Apply animation curve values */
			// The curve current values are updated in the animation notification states
			// Animations are applied in this class as it makes sense to apply the animation movement per animation tick
			if (animatedAvatar->avatarIsInDodge()) {

				// Apply animation curve movement
				animatedAvatar->applyAnimMovement();
			}

			if (animatedAvatar->avatarIsInAttackMotion()) {

				// Apply animation curve movement
				animatedAvatar->applyAnimMovement();

				// Set the right hand speed 
				animatedAvatar->setRighthandResultantSpeed(righthandMovementCurveCurrentValue);
			}
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

void USPSAnimInstance::setCurrentAnimationBase(UAnimSequenceBase* Animation) {
	currentlyPlayingAnimation = Animation;
}