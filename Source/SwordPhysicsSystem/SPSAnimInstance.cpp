// Fill out your copyright notice in the Description page of Project Settings.


#include "SPSAnimInstance.h"

USPSAnimInstance::USPSAnimInstance() {

	// Set animinstance class attributes
	speed = 0.f;
}

USPSAnimInstance::~USPSAnimInstance() {

}


void USPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {

	// Call super function of this function 
	UAnimInstance::NativeUpdateAnimation(DeltaSeconds);
	
	// Actor that is perfoming the animation
	AActor* animatedActor = GetOwningActor();

	// Check if actor exists so to not crash if not
	if (animatedActor != nullptr) {

		// Get the speed. Size as getvelocity returns an FVector
		speed = animatedActor->GetVelocity().Size(); 
	}

}