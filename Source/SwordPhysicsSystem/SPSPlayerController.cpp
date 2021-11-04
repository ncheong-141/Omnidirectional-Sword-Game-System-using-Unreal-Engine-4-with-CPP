// Fill out your copyright notice in the Description page of Project Settings.


#include "SPSPlayerController.h"

// Game classes
#include "Avatar.h"

// Debug file
#include "DebugOutput.h"

#include "GameFramework/InputSettings.h"
#include "Components/InputComponent.h"


ASPSPlayerController::ASPSPlayerController() {

	// Instantiate current keys pressed set
	currentKeysPressed = TArray<KeyPressed>();

	// Set input characteristics
	keyPressTimeInMemory = 0.2;
};





void ASPSPlayerController::PlayerTick(float DeltaTime) {
	
	// Call parent class player tick initially
	APlayerController::PlayerTick(DeltaTime); 

	DebugOutput output = DebugOutput();

	// Iterate over currentKeysPressed and reduce the lifetime in memory. If lifetime over then remove from currentKeysPressed set
	// Iterate back to front such that the removal will not be an issue for indexing
	for (int i = currentKeysPressed.Num() - 1; i >= 0; i--) {
		
		// Update the lifetime of the KeyPressed
		currentKeysPressed[i].updateLifetime(DeltaTime);

		// If the lifetime of the key is done, remove. 
		if (currentKeysPressed[i].lifetime <= 0) {
			currentKeysPressed.RemoveAt(i);
		}
	}

	/* Setting currentKeysPressed based on user input*/
	// Set current key by checking if any of the key keys (lol) is pressed
	// Turn this into an iterable datastructure later

	// Using TArray predicate function. 
	// the [&] in the lambda body tells the compiler to capture outer variables by reference
	// The () is the parameters (Keypress) and condition in curly brackets.
	if (IsInputKeyDown(EKeys::LeftShift)) {
		if (!currentKeysPressed.ContainsByPredicate([&](KeyPressed keyP) {return keyP.key == EKeys::LeftShift; })) {
			currentKeysPressed.Emplace(EKeys::LeftShift, keyPressTimeInMemory);
		}
	}
	if (IsInputKeyDown(EKeys::A)) {
		if (!currentKeysPressed.ContainsByPredicate([&](KeyPressed keyP) {return keyP.key == EKeys::A || keyP.key == EKeys::D || keyP.key == EKeys::W || keyP.key == EKeys::S; })) {
			currentKeysPressed.Emplace(EKeys::A, keyPressTimeInMemory);
		}
	}
	if (IsInputKeyDown(EKeys::D)) {
		if (!currentKeysPressed.ContainsByPredicate([&](KeyPressed keyP) {return keyP.key == EKeys::A || keyP.key == EKeys::D || keyP.key == EKeys::W || keyP.key == EKeys::S; })) {
			currentKeysPressed.Emplace(EKeys::D, keyPressTimeInMemory);
		}
	}
	if (IsInputKeyDown(EKeys::W)) {
		if (!currentKeysPressed.ContainsByPredicate([&](KeyPressed keyP) {return keyP.key == EKeys::A || keyP.key == EKeys::D || keyP.key == EKeys::W || keyP.key == EKeys::S; })) {
			currentKeysPressed.Emplace(EKeys::W, keyPressTimeInMemory);
		}
	}
	if (IsInputKeyDown(EKeys::S)) {
		if (!currentKeysPressed.ContainsByPredicate([&](KeyPressed keyP) {return keyP.key == EKeys::A || keyP.key == EKeys::D || keyP.key == EKeys::W || keyP.key == EKeys::S; })) {
			currentKeysPressed.Emplace(EKeys::S, keyPressTimeInMemory);
		}
	}

	output.toHUD(FString("----"), 2.f, true);
	for (int i = 0; i < currentKeysPressed.Num(); i++) {
		output.toHUD(FString("CurrentKey:" + currentKeysPressed[i].key.GetFName().ToString()), 2.f, false);
	}
	output.toHUD(FString("----"), 2.f, true);

}


const TArray<KeyPressed>ASPSPlayerController::getCurrentKeysPressed() {
	return currentKeysPressed;
}


// Check if the dodge key is active for all movement functions
// This function in here is used in all movement functions and here for coherence
bool ASPSPlayerController::dodgeKeyActive() {
	
	//// Get input settings and Dodge action mappings
	//UInputSettings* inputSettings = UInputSettings::GetInputSettings();
	//TArray<FInputActionKeyMapping> dodgeActionMappings; 
	//inputSettings->GetActionMappingByName("Dodge", dodgeActionMappings);

	//// Debug
	//DebugOutput output = DebugOutput();
	//
	//output.toHUD(dodgeActionMappings[dodgeActionMappings.Num()-1].Key.GetFName().ToString(), 2.f, false);
	//
	//output.toHUD(FString("Current key: " + currentKeyPressed->GetFName().ToString()), 2.f, false);
	//output.toHUD(FString("Last key: " + lastKeyPressed->GetFName().ToString()), 2.f, false);


	//// Get current and last key pressed from custom player controller and compared with dodge key (last key in dodge action mapping data)
	//if (*(currentKeyPressed) == dodgeActionMappings[dodgeActionMappings.Num() - 1].Key || *(lastKeyPressed) == dodgeActionMappings[dodgeActionMappings.Num() - 1].Key) {
	//	return true;
	//}

	return false; 
}