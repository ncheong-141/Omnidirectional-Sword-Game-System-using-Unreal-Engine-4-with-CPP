// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimNotifyState.h"
#include "Avatar.h"
#include "SPSAnimInstance.h"

void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, __FUNCTION__);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, Animation->GetName());
	UE_LOG(LogTemp, Display, TEXT("Notification function: Begin"));
	UE_LOG(LogTemp, Display, TEXT("Animation name: %s"), *(Animation->GetName()));


	// Get the avatar and AnimInstance references to store in class for future use
	// Initial checks
	if (MeshComp != nullptr) {

		// Check for animation instance
		if (MeshComp->GetAnimInstance() != nullptr) {

			// Cast to SPS anim instance
			avatarAnimInstance = Cast<USPSAnimInstance>(MeshComp->GetAnimInstance());

			// Check if cast is successful
			if (avatarAnimInstance != nullptr) {

				// Set flag to true for tick function
				avatarAnimInstanceCastSuccessFlag = true;

				// Reet the time stats
				avatarAnimInstance->totalAnimationDuration = TotalDuration;

				// Set the float curve data to anim instance
				if (Animation != nullptr) {

					// For movement curves
					avatarAnimInstance->setMovementFloatCurvePointers(Animation);

					// For attack curves
					avatarAnimInstance->setAttackFloatCurvePointers(Animation);
				}
			}
			else {
				// Set flag to false for tick function
				avatarAnimInstanceCastSuccessFlag = false;
				UE_LOG(LogTemp, Error, TEXT("SPSAnimInstance cast unsuccessful in %s"), __FUNCTION__);
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("MeshComp->GetAnimInstance() is nullptr in %s"), __FUNCTION__)
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("MechComp is nullptr in %s"), __FUNCTION__)
	}
}


void UAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {

	// Check if avatar and animation instance cast was successful 
	if (avatarAnimInstanceCastSuccessFlag) {
		// Perform code
		// Can perform audio cues here too 

		// Set animation playing to true
		if (avatarAnimInstance) {
			avatarAnimInstance->animationCurrentlyPlaying = true;
		}
	}
}


void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, __FUNCTION__);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, Animation->GetName());
	UE_LOG(LogTemp, Display, TEXT("Notification function: End"));
	UE_LOG(LogTemp, Display, TEXT("Animation name: %s"), *(Animation->GetName()));

	if (avatarAnimInstanceCastSuccessFlag) {

		// Set playing animation to false after finishing animation
		if (avatarAnimInstance) {
			avatarAnimInstance->animationCurrentlyPlaying = false;
		}
	}
}
