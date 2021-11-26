// Fill out your copyright notice in the Description page of Project Settings.


#include "CanDamageAnimNotifyState_Trail.h"
#include "Avatar.h"
#include "SPSAnimInstance.h"

void UCanDamageAnimNotifyState_Trail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, __FUNCTION__);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, Animation->GetName());
	UE_LOG(LogTemp, Display, TEXT("Can Damage Notification function: Begin"));
	UE_LOG(LogTemp, Display, TEXT("Animation name: %s"), *(Animation->GetName()));

	UAnimNotifyState_Trail::NotifyBegin(MeshComp, Animation, TotalDuration);

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
				//Check if inAttackMOtion
				if (avatar->isInAttackMotion) {

					// Set melee weapon can damage to true
					avatar->MeleeWeapon->canDamage = true;
				}
			}
			else {

				UE_LOG(LogTemp, Error, TEXT("Avatar cast unsuccessful in %s"), __FUNCTION__)
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("MechComp->GetOwner() is nullptr in %s"), __FUNCTION__)
		}

		// Set trail properties
		FirstSocketName = FName("SwordTop");
		SecondSocketName = FName("SwordBot");

		// Not sure how to set Trail animation in C++....
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("MechComp is nullptr in %s"), __FUNCTION__)
	}
}


void UCanDamageAnimNotifyState_Trail::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {

	UAnimNotifyState_Trail::NotifyTick(MeshComp, Animation, FrameDeltaTime);

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


void UCanDamageAnimNotifyState_Trail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, __FUNCTION__);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, Animation->GetName());
	UE_LOG(LogTemp, Display, TEXT("Can Damage NotificationNotification function: End"));
	UE_LOG(LogTemp, Display, TEXT("Animation name: %s"), *(Animation->GetName()));

	UAnimNotifyState_Trail::NotifyEnd(MeshComp, Animation);

	// Set melee weapon can damage to false since ending canDamage state
	if (avatar) {
		avatar->MeleeWeapon->canDamage = false;
	}

}


