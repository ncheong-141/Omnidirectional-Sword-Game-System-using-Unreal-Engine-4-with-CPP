// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_StaggeredAnimNotifyState.h"
#include "NPC.h"

void UNPC_StaggeredAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {


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
				//npc->setHasBeenHit(true);
			}
			else {
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


void UNPC_StaggeredAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {


}


void UNPC_StaggeredAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

	// Reset back to false affter animation complete
	if (npc) {
		npc->setHasBeenHit(false);
	}

}

