// Fill out your copyright notice in the Description page of Project Settings.


#include "DodgeAnimNotifyState.h"
#include "Avatar.h"
#include "SPSAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"


void UDodgeAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {

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
				
				// Set avatar to change yaw with camera direction for all dodges
				avatar->bUseControllerRotationYaw = true;
				avatar->avatarMovementComponent->bOrientRotationToMovement = false;
				
				// Set avatar to is in Iframe
				avatar->setAvatarIsInIframe(true);
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

				// Set the time stats
				avatarAnimInstance->currentTime = 0;
				avatarAnimInstance->totalNotificationDuration = TotalDuration;

				// Set the float curve data to anim instance
				if (Animation != nullptr) {
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


void UDodgeAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {
	

	// Check if avatar and animation instance cast was successful 

	// Perform code
	// Can perform audio cues here too 

	// Set animation playing to true
	if (avatarAnimInstance) {
		avatarAnimInstance->animationCurrentlyPlaying = true;
	}	

	// Set avatar to is in Iframe
	if (avatar) {
		avatar->setAvatarIsInIframe(true);
	}

}


void UDodgeAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	
	// Set dodge to false as it has ended
	if (avatar) {
		avatar->setAvatarIsInDodge(false);
		avatar->setAvatarIsInIframe(false);

		// Reset camera back to normal
		avatar->bUseControllerRotationYaw = false;
		avatar->avatarMovementComponent->bOrientRotationToMovement = true;
	}

	// Set playing animation to false after finishing animation
	if (avatarAnimInstance) {
		avatarAnimInstance->animationCurrentlyPlaying = false;
	}
}