// Fill out your copyright notice in the Description page of Project Settings.


#include "SPSPlayerController.h"

// Debug file
#include "DebugOutput.h"

ASPSPlayerController::ASPSPlayerController() {

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
};





void ASPSPlayerController::PlayerTick(float DeltaTime) {
	
	// Call parent class player tick initially
	APlayerController::PlayerTick(DeltaTime); 

	//DebugOutput output = DebugOutput();

	/* Set the last key pressed to current key pressed AFTER A CERTAIN DELAY(lastKeyOverwriteDelayValue) since this is the next tick
	   and current key was set from last tick */

	//  Check if the current key is different from the last key first before considering calculating the delay
	

	// If the time has come to overwrite the lastKeyPressed
	if (lastKeyTimeTillOverwrite <= 0) {

		// Delay over, set last key to current key
		lastKeyPressed = &*(currentKeyPressed);

		// Reset the time to delay 
		lastKeyTimeTillOverwrite = lastKeyOverwriteDelayValue;
	}
	else {
		// Reduce the time till overwrite by the time difference between this and last frame. 
		lastKeyTimeTillOverwrite -= DeltaTime;
	}

	/* Setting currentKeyPressed based on user input*/
	// Set current key by checking if any of the key keys (lol) is pressed
	if (IsInputKeyDown(EKeys::A)) {
		currentKeyPressed = &A_FKey;
	}
	else if (IsInputKeyDown(EKeys::D)) {
		currentKeyPressed = &D_FKey;
	}
	else if (IsInputKeyDown(EKeys::W)) {
		currentKeyPressed = &W_FKey;
	}
	else if (IsInputKeyDown(EKeys::S)) {
		currentKeyPressed = &S_FKey;
	}
	else {
		currentKeyPressed = &empty_FKey;
	}

	// Left shift gets priority ( need a better soln for this though)
	if (IsInputKeyDown(EKeys::LeftShift)) {
		currentKeyPressed = &LShift_FKey;
	}
}


const FKey* ASPSPlayerController::getLastKeyPressed() {
	return lastKeyPressed;
}

const FKey* ASPSPlayerController::getCurrentKeyPressed() {
	return currentKeyPressed;
}
