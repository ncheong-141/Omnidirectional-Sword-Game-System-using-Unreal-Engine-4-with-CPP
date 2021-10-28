// Fill out your copyright notice in the Description page of Project Settings.


#include "SPSAnimInstance.h"

// Game classes
#include "Avatar.h"

// Unreal engine files
#include "GameFramework/CharacterMovementComponent.h"



USPSAnimInstance::USPSAnimInstance() {

	// Set animinstance class attributes
	speed = 0.f;
	isInAir = false;
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

		// Get the speed. Size as getvelocity returns an FVector
		speed = animatedAvatar->GetVelocity().Size(); 

		// Check if actor is in air
		isInAir = animatedAvatar->GetCharacterMovement()->IsFalling();
		
	}

}