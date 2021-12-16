// Fill out your copyright notice in the Description page of Project Settings.


#include "UnbalancedByBlockAnimNotifyState.h"
#include "NPC.h"

void UUnbalancedByBlockAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {


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


void UUnbalancedByBlockAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {


}


void UUnbalancedByBlockAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {


	if (weaponHolder) {

		if (Cast<ANPC>(weaponHolder.GetObject())) {

			ANPC* npc = Cast<ANPC>(weaponHolder.GetObject());
			npc->setIsunbalancedFromBlock(false);
		}
	}
}