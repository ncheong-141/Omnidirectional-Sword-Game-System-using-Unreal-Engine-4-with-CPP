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
	TArray<EKeys> keysPressed; 

public:

	// Constructors. Note, super constructor required to be called 
	ASPSPlayerController() {};
	ASPSPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}


	/**
	* Processes player input (immediately after PlayerInput gets ticked) and calls UpdateRotation().
	* PlayerTick is only called if the PlayerController has a PlayerInput object. Therefore, it will only be called for locally controlled PlayerControllers.
	*/
	virtual void PlayerTick(float DeltaTime) override;

};
