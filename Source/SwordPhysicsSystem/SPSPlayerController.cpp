// Fill out your copyright notice in the Description page of Project Settings.


#include "SPSPlayerController.h"

// Game classes
#include "Avatar.h"

// Debug file
#include "DebugOutput.h"

#include "Components/InputComponent.h"


ASPSPlayerController::ASPSPlayerController() {


	// Get input settings for reference
	inputSettings = UInputSettings::GetInputSettings();

	// Set up dodge action mappings
	inputSettings->GetActionMappingByName("Dodge", dodgeActionMappings);
};




void ASPSPlayerController::PlayerTick(float DeltaTime) {
	
	// Call parent class player tick initially
	APlayerController::PlayerTick(DeltaTime); 
}




/* Old key press system (in Tick)*/

//// Iterate over currentKeysPressed and reduce the lifetime in memory. If lifetime over then remove from currentKeysPressed set
//// Iterate back to front such that the removal will not be an issue for indexing
//for (int i = currentKeysPressed.Num() - 1; i >= 0; i--) {
//
//	// Update the lifetime of the KeyPressed
//	currentKeysPressed[i].updateLifetime(DeltaTime);
//
//	// If the lifetime of the key is done, remove. 
//	if (currentKeysPressed[i].lifetime <= 0) {
//		currentKeysPressed.RemoveAt(i);
//	}
//}
///* Setting currentKeysPressed based on user input*/
//// Set current key by checking if any of the key keys (lol) is pressed
//// Turn this into an iterable datastructure later
//
//// Using TArray predicate function. 
//// the [&] in the lambda body tells the compiler to capture outer variables by reference
//// The () is the parameters (Keypress) and condition in curly brackets.
//
//// Only allow for 1 WASD input to be stored at a time. !!!This can be changed to a queue structure in the future. !!!
//if (IsInputKeyDown(EKeys::LeftShift)) {
//	if (!currentKeysPressed.ContainsByPredicate([&](KeyPressed keyP) {return keyP.key == EKeys::LeftShift; })) {
//		currentKeysPressed.Emplace(EKeys::LeftShift, keyPressTimeInMemory);
//	}
//}
//if (IsInputKeyDown(EKeys::A)) {
//	if (!currentKeysPressed.ContainsByPredicate([&](KeyPressed keyP) {return keyP.key == EKeys::A || keyP.key == EKeys::D || keyP.key == EKeys::W || keyP.key == EKeys::S; })) {
//		currentKeysPressed.Emplace(EKeys::A, keyPressTimeInMemory);
//	}
//}
//if (IsInputKeyDown(EKeys::D)) {
//	if (!currentKeysPressed.ContainsByPredicate([&](KeyPressed keyP) {return keyP.key == EKeys::A || keyP.key == EKeys::D || keyP.key == EKeys::W || keyP.key == EKeys::S; })) {
//		currentKeysPressed.Emplace(EKeys::D, keyPressTimeInMemory);
//	}
//}
//if (IsInputKeyDown(EKeys::W)) {
//	if (!currentKeysPressed.ContainsByPredicate([&](KeyPressed keyP) {return keyP.key == EKeys::A || keyP.key == EKeys::D || keyP.key == EKeys::W || keyP.key == EKeys::S; })) {
//		currentKeysPressed.Emplace(EKeys::W, keyPressTimeInMemory);
//	}
//}
//if (IsInputKeyDown(EKeys::S)) {
//	if (!currentKeysPressed.ContainsByPredicate([&](KeyPressed keyP) {return keyP.key == EKeys::A || keyP.key == EKeys::D || keyP.key == EKeys::W || keyP.key == EKeys::S; })) {
//		currentKeysPressed.Emplace(EKeys::S, keyPressTimeInMemory);
//	}
//}
//
////if (currentKeysPressed.Num() >= 1) {
////	output.toHUD(FString("----"), 2.f, true);
////	for (int i = 0; i < currentKeysPressed.Num(); i++) {
////		output.toHUD(FString("CurrentKey:" + currentKeysPressed[i].key.GetFName().ToString()), 2.f, false);
////	}
////	output.toHUD(FString("----"), 2.f, true);
////}

//// Internal struct
//// Purpose of this class is to hold a key information and attach a lifetime onto it
//struct KeyPressed {
//
//	// Attribute
//	FKey key;
//	float lifetime;
//
//	KeyPressed(FKey key_, float lifetime_) : key(key_),
//		lifetime(lifetime_) {}
//
//	// Struct functions 
//	void updateLifetime(float DeltaTime) {
//		lifetime -= DeltaTime;
//	}
//};