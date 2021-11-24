// Fill out your copyright notice in the Description page of Project Settings.


#include "DodgeAnimNotifyState.h"
#include "Avatar.h"
#include "SPSAnimInstance.h"

void UDodgeAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, __FUNCTION__);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, Animation->GetName());
	UE_LOG(LogTemp, Display, TEXT("Notification function: Begin"))
	UE_LOG(LogTemp, Display, TEXT("Animation name: %s"), *(Animation->GetName()))


	// Get the avatar and AnimInstance references to store in class for future use
	// Initial checks
	if (MeshComp != nullptr) {

		// Check for owner
		if (MeshComp->GetOwner() != nullptr) {
				
			// Cast to avatar and set up class variable
			avatar = Cast<AAvatar>(MeshComp->GetOwner());

			// Check if cast is successful 
			if (avatar != nullptr) {
				
				// Set flag to true
				avatarCastSuccessFlag = true;

				// Begin notifcation code for avatar
				// Not required to set isInDodge for avatar as done in input command
			}
			else {

				// Cast failed set flag to false so it does not run in tick function
				avatarCastSuccessFlag = false;
				UE_LOG(LogTemp, Error, TEXT("Avatar cast unsuccessful in %s"), __FUNCTION__)
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("MechComp->GetOwner() is nullptr in %s"), __FUNCTION__)
		}


		// Check for animation instance
		if (MeshComp->GetAnimInstance() != nullptr) {

			// Cast to SPS anim instance
			avatarAnimInstance = Cast<USPSAnimInstance>(MeshComp->GetAnimInstance());

			// Check if cast is successful
			if (avatarAnimInstance != nullptr) {

				// Set flag to true for tick function
				avatarAnimInstanceCastSuccessFlag = true; 

				// Reset the readings in AnimInstance for this animation
				avatarAnimInstance->resetReadingOfAnimationCurve = true;

				// Set animation playing to true
				avatarAnimInstance->animationCurrentlyPlaying = true;

				// Set the time stats
				avatarAnimInstance->currentTime = 0;
				avatarAnimInstance->totalAnimationDuration = TotalDuration;

				// Set the float curve data to anim instance
				if (Animation != nullptr) {
					avatarAnimInstance->setFloatCurvePointers(Animation);
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


void UDodgeAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {
	
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, __FUNCTION__);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, Animation->GetName());
	UE_LOG(LogTemp, Display, TEXT("Notification function: Tick"));
	UE_LOG(LogTemp, Display, TEXT("Animation name: %s"), *(Animation->GetName()));


	// Check if avatar and animation instance cast was successful 
	if (avatarCastSuccessFlag && avatarAnimInstanceCastSuccessFlag) {
		// Perform code
		// Can perform audio cues here too 
	}

	// No need for error message as it will be thrown in NotifyBegin()
}


void UDodgeAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, __FUNCTION__);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, Animation->GetName());


	if (avatarCastSuccessFlag && avatarAnimInstanceCastSuccessFlag) {
		
		// Set dodge to false as it has ended
		avatar->isInDodge = false;

		// Set playing animation to false after finishing animation
		avatarAnimInstance->animationCurrentlyPlaying = false;
	}


}