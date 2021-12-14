// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Containers/Array.h"
#include "GameFramework/InputSettings.h"
#include "SPSPlayerController.generated.h"

/**
 *  This class is set in the BP_GameModeSPS. It automaticalled assigns this as the PlayerController (I think)
	
	Purpose of this class is to add functionality to the PlayerController, 
	Mainly to store input settings which can be refrenced throughout the game when required.
	A further usage would be to store key presses to allow for multiple key press actions.
 */

// Forward declarations
class AAvatar;
class UInputSettings; 


UCLASS()
class SWORDPHYSICSSYSTEM_API ASPSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:


	// Key variable pointers which switch between FKey instances (no copying required)
	// Will likely change this to a Queue Storage in the future
	//TArray<KeyPressed> currentKeysPressed;

	// Key "sensitivity" to change
	//float keyPressTimeInMemory; 

public:

	/* Public class attributes used in Sword/Avatar methods for input informations */
	
	// Reference to input settings
	UPROPERTY()	
		UInputSettings* inputSettings;

	// Input mappings for reference
	UPROPERTY()
		TArray<FInputActionKeyMapping> dodgeActionMappings;

	UPROPERTY()
		TArray<FInputActionKeyMapping> stanceSwitchMappings; 


	// Constructors. Note, super constructor required to be called in order to set up the controller. 
	ASPSPlayerController();
	ASPSPlayerController(const FObjectInitializer& ObjectInitializer);

	/**
	* Processes player input (immediately after PlayerInput gets ticked) and calls UpdateRotation().
	* PlayerTick is only called if the PlayerController has a PlayerInput object. Therefore, it will only be called for locally controlled PlayerControllers.
	*/
	virtual void PlayerTick(float DeltaTime) override;

};
