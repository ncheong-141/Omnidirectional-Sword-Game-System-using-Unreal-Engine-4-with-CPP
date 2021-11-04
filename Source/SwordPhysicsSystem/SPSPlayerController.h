// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Containers/Array.h"
#include "Components/InputComponent.h"
#include "SPSPlayerController.generated.h"

/**
 *  This class is set in the BP_GameModeSPS. It automaticalled assigns this as the PlayerController (I think)
	
	Purpose of this class is to add functionality to the PlayerController, mainly to be able to
	store key presses such that inputs can be more flexible (e.g. press A then shift to dodge) 
 */
UCLASS()
class SWORDPHYSICSSYSTEM_API ASPSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:


	// Instances of keys being monitored such that only one instance is requried
	FKey	empty_FKey;
	FKey	A_FKey; 
	FKey	D_FKey;
	FKey	W_FKey;
	FKey	S_FKey;
	FKey	LShift_FKey; 

	// Key variable pointers which switch between FKey instances (no copying required)
	// Will likely change this to a Queue Storage in the future
	FKey* lastKeyPressed;
	FKey* currentKeyPressed;


	// Key "sensitivity" to change
	float lastKeyOverwriteDelayValue; 
	float lastKeyTimeTillOverwrite;

public:

	// Constructors. Note, super constructor required to be called in order to set up the controller. 
	ASPSPlayerController();
	ASPSPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

		// Initialise key variables such that no copying is required and checks are done on a single instance of each key
		A_FKey = EKeys::A;
		D_FKey = EKeys::D; 
		W_FKey = EKeys::W;
		S_FKey = EKeys::S;
		LShift_FKey = EKeys::LeftShift;
		empty_FKey = FKey(); 

		// Set pointers to none
		lastKeyPressed = &empty_FKey;
		currentKeyPressed = &empty_FKey;

		// Set input characteristics
		lastKeyOverwriteDelayValue = 0.1;
		lastKeyTimeTillOverwrite = lastKeyOverwriteDelayValue;
	}

	/**
	* Processes player input (immediately after PlayerInput gets ticked) and calls UpdateRotation().
	* PlayerTick is only called if the PlayerController has a PlayerInput object. Therefore, it will only be called for locally controlled PlayerControllers.
	
	This function will be used to store the last and current key pressed. Only certain keys will be checked for. 
	*/
	virtual void PlayerTick(float DeltaTime) override;

	const FKey* getLastKeyPressed();
	const FKey* getCurrentKeyPressed();

};
