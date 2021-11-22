// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashSwordStance.h"
#include "Avatar.h"
#include "DebugOutput.h"

// Constructor and destructor implementation

SlashSwordStance::~SlashSwordStance()
{
}

// Virtual function override class function implementation
void SlashSwordStance::displayStance() {

	DebugOutput output = DebugOutput();

	output.toHUD(FString("Slash Stance"), 2.f, false);
}


void SlashSwordStance::Yaw(float amount) {

	// Change sword position based on mouse position
	avatarPtr->swordFocalPoint->update(avatarPtr->pController);
}

void SlashSwordStance::Pitch(float amount) {
	// Change sword position based on mouse position
	avatarPtr->swordFocalPoint->update(avatarPtr->pController);
}