// Fill out your copyright notice in the Description page of Project Settings.
#include "DefaultSwordStance.h"

// Debug output file
#include "DebugOutput.h"

// Constructor and destructor implementation
DefaultSwordStance::DefaultSwordStance() {

}

DefaultSwordStance::DefaultSwordStance(AAvatar* avatar, int stance_ID, bool applyRotationToSFP) : SwordStance(avatar, stance_ID, applyRotationToSFP) {

}


DefaultSwordStance::~DefaultSwordStance() {

}

// Virtual function override class function implementation
void DefaultSwordStance::displayStance() {

	DebugOutput output = DebugOutput(); 
	
	output.toHUD(FString("Default Stance"), 2.f, false);
}

void DefaultSwordStance::swordStanceActivation() {

	// Nothing just yet
}


void DefaultSwordStance::swordStanceDeactivation() {

	// Nothing just yet
}