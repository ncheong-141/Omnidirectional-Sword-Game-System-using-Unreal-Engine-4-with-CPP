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

	test = false;
}

USPSAnimInstance::~USPSAnimInstance() {

}


void USPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {

	// Call super function of this function 
	UAnimInstance::NativeUpdateAnimation(DeltaSeconds);
	
	if (animatedActorTest == nullptr) {
		animatedActorTest = GetOwningActor();
	} 

	// Actor that is perfoming the animation
	AActor* animatedActor = GetOwningActor();

	// Check if actor exists so to not crash if not
	if (animatedActor != nullptr) {

		// Get the speed. Size as getvelocity returns an FVector
		speed = animatedActor->GetVelocity().Size(); 

		// Cast actor to Avatar to access derived class functionality 
		AAvatar* animatedAvatar = Cast<AAvatar>(animatedActor);

		// Check if cast was successful
		if (animatedAvatar != nullptr) {

			// Check if actor is in air
			isInAir = animatedAvatar->GetCharacterMovement()->IsFalling();
		}
	}

}