// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebugHUD.h"


/**
 * DebugOutput class. 
 * This class is used to handle all debug output to the console or ingame DebugHUD.
 * It includes the set up of the DebugHUD such that no other class is required to do so. 
 * Other game classes only need to call a member function of this class to output debug information. 

 */

class SWORDPHYSICSSYSTEM_API DebugOutput {
private: 

	// Class attributes
	ADebugHUD* debugHUD; 

public:

	// Constructor 
	DebugOutput();

	/* Member functions */
	void toHUD(FString message, float time, bool isProblem);
	void toConsole(const char* message, bool isProblem); 
};
