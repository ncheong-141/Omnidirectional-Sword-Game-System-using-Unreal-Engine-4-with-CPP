// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAttackAnimNotifyState.h"
#include "NPC.h"

void UNPCAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {

	// Get the avatar and AnimInstance references to store in class for future use
	// Initial checks
	if (MeshComp != nullptr) {

		// Check for owner
		if (MeshComp->GetOwner() != nullptr) {

			// Cast to avatar and set up class variable
			npc = Cast<ANPC>(MeshComp->GetOwner());

			// Check if cast is successful 
			if (npc != nullptr) {

				// Begin notifcation code for avatar
				// Not required to set isInDodge for avatar as done in input command
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("NPC cast unsuccessful in %s"), __FUNCTION__)
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


void UNPCAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {


}


void UNPCAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

	// Set is in attack motion to false
	if (npc) {
		npc->setIsInAttackMotion(false);
	}

}
