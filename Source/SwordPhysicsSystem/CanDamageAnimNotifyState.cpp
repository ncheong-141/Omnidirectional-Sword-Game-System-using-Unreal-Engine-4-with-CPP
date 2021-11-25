// Fill out your copyright notice in the Description page of Project Settings.


#include "CanDamageAnimNotifyState.h"
#include "Avatar.h"
#include "SPSAnimInstance.h"

void UCanDamageAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, __FUNCTION__);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, Animation->GetName());
	UE_LOG(LogTemp, Display, TEXT("Notification function: Begin"));
	UE_LOG(LogTemp, Display, TEXT("Animation name: %s"), *(Animation->GetName()));


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
				//Check if inAttackMOtion
				if (avatar->isInAttackMotion) {

					// Set melee weapon can damage to true
					avatar->MeleeWeapon->canDamage = true;
				}
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
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("MechComp is nullptr in %s"), __FUNCTION__)
	}
}


void UCanDamageAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {

	// Check if avatar and animation instance cast was successful 
	if (avatarCastSuccessFlag) {
		// Perform code
		// Can perform audio cues here too 

		// Constantly setting to true as animation may change/blend, this would invoke 
		// the NotifyEnd() method of another canDamageState and thus this needs to be updated regularly
		if (avatar) {

			//Check if inAttackMOtion
			if (avatar->isInAttackMotion) {

				// Set melee weapon can damage to true
				avatar->MeleeWeapon->canDamage = true;
			}
		}
	}
}


void UCanDamageAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, __FUNCTION__);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, Animation->GetName());
	UE_LOG(LogTemp, Display, TEXT("Notification function: End"));
	UE_LOG(LogTemp, Display, TEXT("Animation name: %s"), *(Animation->GetName()));

	if (avatarCastSuccessFlag) {

		// Set melee weapon can damage to false since ending canDamage state
		if (avatar) {
			avatar->MeleeWeapon->canDamage = false;
		}
	}
}

