// Fill out your copyright notice in the Description page of Project Settings.
#include "DefaultSwordStance.h"

// Debug output file
#include "DebugOutput.h"

// Constructor and destructor implementation
DefaultSwordStance::~DefaultSwordStance() {

}

// Virtual function override class function implementation
void DefaultSwordStance::displayStance() {

	DebugOutput output = DebugOutput(); 
	
	output.toHUD(FString("Default"), 2.f, false);
}