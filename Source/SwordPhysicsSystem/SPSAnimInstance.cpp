// Fill out your copyright notice in the Description page of Project Settings.


#include "SPSAnimInstance.h"

// Game classes
#include "Avatar.h"

// Unreal engine files
#include "GameFramework/CharacterMovementComponent.h"



USPSAnimInstance::USPSAnimInstance() {
	animatedAvatar = nullptr;
	currentAnimMontage = nullptr;
}

USPSAnimInstance::~USPSAnimInstance() {

}


void USPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {

	// Call super function of this function 
	UAnimInstance::NativeUpdateAnimation(DeltaSeconds);
	
	// Set up avatar reference if its the first frame
	// (cannot do this in post initialisation as Avatar instance does not exist yet..
	if (animatedAvatar == nullptr) {
		
		// Get the object
		AActor* animatedActor = GetOwningActor();

		if (animatedActor != nullptr) {
			// Cast to avatar
			animatedAvatar = Cast<AAvatar>(animatedActor);
			
			// If the cast failed, return
			if (animatedAvatar == nullptr) {
				return; 
			}
		}
		else {
			return; 
		}
	} 

	// Check if actor exists so to not crash if not
	if (animatedAvatar != nullptr) {

		UAnimInstance* avatarAnimInstance = animatedAvatar->GetMesh()->GetAnimInstance();

		// Get the Animation montage (blended animation sequences currently playing)
		//UAnimMontage* currentAnimMontage = avatarAnimInstance->GetCurrentActiveMontage();

		while (!(currentAnimMontage == nullptr)) {

			UE_LOG(LogTemp, Display, TEXT("Montage not Null"));
			auto avatarAnimCurves = currentAnimMontage->GetCurveData().GetCurveData(0);
			if (avatarAnimCurves != nullptr) {
				UE_LOG(LogTemp, Display, TEXT("Curve name: %s"), *(avatarAnimCurves->Name.DisplayName.ToString()));
			}
			else {
				UE_LOG(LogTemp, Display, TEXT("Curve data Null"));
			}
		}
	}
}