// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Containers/Array.h"
#include "GameFramework/InputSettings.h"
#include "SPSPlayerController.generated.h"

/**
 *  This class is set in the BP_GameModeSPS. It automaticalled assigns this as the PlayerController (I think)
	
	Purpose of this class is to add functionality to the PlayerController, mainly to be able to
	store key presses such that inputs can be more flexible (e.g. press A then shift to dodge) 
 */

// Forward declarations
class AAvatar;
class UInputSettings; 

// Internal struct
// Purpose of this class is to hold a key information and attach a lifetime onto it
struct KeyPressed {

	// Attribute
	FKey key; 
	float lifetime; 

	KeyPressed(FKey key_, float lifetime_) :	key(key_), 
													lifetime(lifetime_) {}

	// Struct functions 
	void updateLifetime(float DeltaTime) {
		lifetime -= DeltaTime;
	}
};

UCLASS()
class SWORDPHYSICSSYSTEM_API ASPSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:


	// Key variable pointers which switch between FKey instances (no copying required)
	// Will likely change this to a Queue Storage in the future
	TArray<KeyPressed> currentKeysPressed;

	// Key "sensitivity" to change
	float keyPressTimeInMemory; 

public:

	// Public class attributes
	// Reference to input settings
	UInputSettings* inputSettings;

	// Constructors. Note, super constructor required to be called in order to set up the controller. 
	ASPSPlayerController();
	ASPSPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

		// Instantiate current keys pressed set
		currentKeysPressed = TArray<KeyPressed>();

		// Get input settings for reference
		inputSettings = UInputSettings::GetInputSettings();

		// Set input characteristics
		keyPressTimeInMemory = 0.5;
	}

	/**
	* Processes player input (immediately after PlayerInput gets ticked) and calls UpdateRotation().
	* PlayerTick is only called if the PlayerController has a PlayerInput object. Therefore, it will only be called for locally controlled PlayerControllers.
	
	This function will be used to store the last and current key pressed. Only certain keys will be checked for. 
	*/
	virtual void PlayerTick(float DeltaTime) override;

	const TArray<KeyPressed> getCurrentKeysPressed();

};
