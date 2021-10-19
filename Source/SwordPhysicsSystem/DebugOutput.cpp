// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugOutput.h"
#include <iostream>


DebugOutput::DebugOutput() {

	// Set up HUD. Using global variable GWorld to get player controller reference
	APlayerController* PController = GWorld->GetWorld()->GetFirstPlayerController();

	// Get refernce of the Hud from the controller
	debugHUD = Cast<ADebugHUD>(PController->GetHUD());
}


void DebugOutput::toHUD(FString message, float time, bool isProblem) {

	// Add debug message to message list to be displayed
	debugHUD->addDebugMessage(DebugMessage(message, time, isProblem));
}

void DebugOutput::toConsole(const char* message, bool isProblem) {

	// Print debug message to console
	if (isProblem) {
		std::cout << "PROBLEM: " << message << std::endl;
	}
	else {
		std::cout << message << std::endl;
	}
}

