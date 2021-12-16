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

			// Calculate correctedNotificaitonDuration based on animation base ratescale and attackspeed
			// Required as AnimationSequence only supplies duration of animation ratescale 1.0 without any modifiers.
			// If attack speed is positive, going in normal animation play direction
			if (animatedAvatar->getAttackSpeed() > 0) {
				
				float correctedNotificationDuration = totalNotificationDuration / (currentlyPlayingAnimation->RateScale * animatedAvatar->getAttackSpeed());

				// Calculate the current time
				// Notificaiton duration scaled by rate scale
				// *0.85 as for some reason current time and anim time does not match up even when not corrected
				// Must be something to do with desync between anim notif state and animinstance deltaseconds..
				// Need a better, more robust soln later
				if (currentTime + DeltaSeconds < 0.85*correctedNotificationDuration) {
					lastFrameTime = currentTime;
					currentTime += DeltaSeconds;
				}
				else {
					//UE_LOG(LogTemp, Error, TEXT("over total time"));
					// End the attack
					animatedAvatar->getStance()->swordStanceDeactivation();
					animationCurrentlyPlaying = false;
				}
				//UE_LOG(LogTemp, Display, TEXT("Current Time: %f"), currentTime);
				//UE_LOG(LogTemp, Display, TEXT("Anim Notif Time: %f"), totalNotificationDuration);
				//UE_LOG(LogTemp, Display, TEXT("Corrected Anim Time: %f"), correctedNotificationDuration);

			}
			else {
				// Negative attack speed, need to count down from current time				
				if (currentTime - DeltaSeconds > 0) {
					lastFrameTime = currentTime;
					currentTime -= DeltaSeconds;
				}
				else {
				//	UE_LOG(LogTemp, Error, TEXT("less than 0 time"));
				}
			}


			/* Reading of curve data */
			// Set curve current and last frame values
			// Note last frame value is obtained from the curve as the curves can switch depending on anim playing
			if (currentlyPlayingAnimation != nullptr) {

				// ForwardMovement distance
				fMovementDistanceCurveLastFrameValue = getFloatValueFromCurve(lastFrameTime, currentlyPlayingAnimation, FName("ForwardMovement"));
				fMovementDistanceCurveCurrentValue = getFloatValueFromCurve(currentTime, currentlyPlayingAnimation, FName("ForwardMovement"));
				
				// Right movement distance			
				rMovementDistanceCurveLastFrameValue = getFloatValueFromCurve(lastFrameTime, currentlyPlayingAnimation, FName("RightMovement"));
				rMovementDistanceCurveCurrentValue = getFloatValueFromCurve(currentTime, currentlyPlayingAnimation, FName("RightMovement"));
		
				// Up movement distance
				upMovementDistanceCurveLastFrameValue = getFloatValueFromCurve(lastFrameTime, currentlyPlayingAnimation, FName("UpMovement"));
				upMovementDistanceCurveCurrentValue = getFloatValueFromCurve(currentTime, currentlyPlayingAnimation, FName("UpMovement"));


				// Right hand movement speed
				righthandMovementCurveCurrentValue = getFloatValueFromCurve(currentTime, currentlyPlayingAnimation, FName("thumb_01_r_MovementSpeed"));
				
			} 
			

			/* Apply animation curve values */
			// The curve current values are updated in the animation notification states
			// Animations are applied in this class as it makes sense to apply the animation movement per animation tick
			if (animatedAvatar->avatarIsInDodge() || animatedAvatar->getHasBeenHit()) {

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

void USPSAnimInstance::setCurrentAnimation(UAnimSequenceBase* Animation) {
	currentlyPlayingAnimation = Cast<UAnimSequence>(Animation);
}

float USPSAnimInstance::getFloatValueFromCurve(float time, UAnimSequence* Sequence, FName CurveName) {

	USkeleton* Skeleton = Sequence->GetSkeleton();

	const FSmartNameMapping* NameMapping = Skeleton->GetSmartNameContainer(USkeleton::AnimCurveMappingName);

	USkeleton::AnimCurveUID Uid;
	Uid = NameMapping->FindUID(CurveName);

	float value = Sequence->EvaluateCurveData(Uid, time);
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, FString::Printf(TEXT("Value from curve: %f"), value));
	
	return value;
}