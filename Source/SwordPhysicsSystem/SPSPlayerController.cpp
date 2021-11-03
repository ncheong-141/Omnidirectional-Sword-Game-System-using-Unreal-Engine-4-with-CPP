// Fill out your copyright notice in the Description page of Project Settings.


#include "SPSPlayerController.h"

// Debug file
#include "DebugOutput.h"


void ASPSPlayerController::PlayerTick(float DeltaTime) {
	
	// Call parent class player tick initially
	APlayerController::PlayerTick(DeltaTime); 

	DebugOutput output = DebugOutput();


	output.toHUD(FString("In SPSPlayerController"), 2.f, false);

}

