// Fill out your copyright notice in the Description page of Project Settings.


#include "WasBlockedAnimNotifyState.h"

#include "Avatar.h"
#include "SPSAnimInstance.h"

void UWasBlockedAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, __FUNCTION__);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, Animation->GetName());
	//UE_LOG(LogTemp, Display, TEXT("Notification function: Begin"));
	//UE_LOG(LogTemp, Display, TEXT("Animation name: %s"), *(Animation->GetName()));


	// Get the avatar and AnimInstance references to store in class for future use
	// Initial checks
	if (MeshComp != nullptr) {

		// Check for owner
		if (MeshComp->GetOwner() != nullptr) {

			// Cast to avatar and set up class variable
			weaponHolder = TScriptInterface<ISPSWeaponHolder>(MeshComp->GetOwner());

			// Check if cast is successful 
			if (weaponHolder != nullptr) {

			}
			else {

				UE_LOG(LogTemp, Error, TEXT("weaponHolder cast unsuccessful in %s"), __FUNCTION__)
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


void UWasBlockedAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {


	// Perform code
	// Can perform audio cues here too 

	// Constantly setting to true as animation may change/blend, this would invoke 
	// the NotifyEnd() method of another canDamageState and thus this needs to be updated regularly

	//Check if inAttackMOtion
	if (weaponHolder) {

		// If weaponholder was blocked 
		if (weaponHolder->SPSActorWasBlocked()) {

			// Set damage to false
			weaponHolder->getMeleeWeapon()->canDamage = false;

			// Reversse attackspeed variable to simulate recoil (no animations for rebound)
			weaponHolder->setAttackSpeed(-1.f); 
		}
	}
}


void UWasBlockedAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, __FUNCTION__);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, Animation->GetName());
	//UE_LOG(LogTemp, Display, TEXT("Notification function: End"));
	//UE_LOG(LogTemp, Display, TEXT("Animation name: %s"), *(Animation->GetName()));


	// Once state is exited either at end or by interuption, set wasblocked to false
	// Good having it here as you can either not input and play full rebound animation or the faster
	// you input the faster you recover from the animation so its responsive
	if (weaponHolder) {

		if (weaponHolder->SPSActorWasBlocked()) {
			weaponHolder->stopAttackIfBlocked();
		}
	}
}