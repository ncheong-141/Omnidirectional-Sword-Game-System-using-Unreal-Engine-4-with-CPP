// Fill out your copyright notice in the Description page of Project Settings.


#include "StaggeredAnimNotifyState.h"
#include "Avatar.h"
#include "SPSAnimInstance.h"

void UStaggeredAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {


	// Get the avatar and AnimInstance references to store in class for future use
	// Initial checks
	if (MeshComp != nullptr) {

		// Check for owner
		if (MeshComp->GetOwner() != nullptr) {

			// Cast to avatar and set up class variable
			avatar = Cast<AAvatar>(MeshComp->GetOwner());

			// Check if cast is successful 
			if (avatar != nullptr) {

				// Begin notifcation code for avatar
				// Not required to set isInDodge for avatar as done in input command
				avatar->setHasBeenHit(true);
			}
			else {
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

				// Reset the time stats
				// Set the time of the notification 
				avatarAnimInstance->totalNotificationDuration = TotalDuration;

				// Set the float curve data to anim instance
				if (Animation != nullptr) {

					// Animation base reference
					avatarAnimInstance->setCurrentAnimation(Animation);
				}
			}
			else {
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


void UStaggeredAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {

	// Set animation playing to true
	if (avatarAnimInstance) {
		avatarAnimInstance->animationCurrentlyPlaying = true;
	}
}


void UStaggeredAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

	// Reset back to false affter animation complete
	if (avatar) {
		avatar->setHasBeenHit(false);
	}

	// Set animation playing to true
	if (avatarAnimInstance) {
		avatarAnimInstance->animationCurrentlyPlaying = false;
	}
}

