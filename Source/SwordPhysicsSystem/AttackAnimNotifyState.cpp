// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimNotifyState.h"
#include "Avatar.h"
#include "SPSAnimInstance.h"

void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {

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

					// For movement curves
					avatarAnimInstance->setMovementFloatCurvePointers(Animation);

					// For attack curves
					avatarAnimInstance->setAttackFloatCurvePointers(Animation);
					
					// Animation base reference
					avatarAnimInstance->setCurrentAnimationBase(Animation);
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


void UAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {


	// Perform code
	// Can perform audio cues here too 
	// Calculate allowable sword directions (this is stance dependent and is called polymorphicaly)
	if (avatar) {
		avatar->getStance()->calculateAllowableSwordDirections();
	}

	// Set animation playing to true
	if (avatarAnimInstance) {
		avatarAnimInstance->animationCurrentlyPlaying = true;
	}
}


void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	
	// Set playing animation to false after finishing animation
	if (avatarAnimInstance) {
		avatarAnimInstance->animationCurrentlyPlaying = false;
	}

	//if (avatar) {
	//	avatar->getStance()->swordStanceDeactivation();
	//}

}
