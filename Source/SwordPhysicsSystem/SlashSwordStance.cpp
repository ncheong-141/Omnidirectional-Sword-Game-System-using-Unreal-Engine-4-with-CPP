// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashSwordStance.h"
#include "DebugOutput.h"

// Constructor and destructor implementation

SlashSwordStance::~SlashSwordStance()
{
}

// Virtual function override class function implementation
void SlashSwordStance::displayStance() {

	DebugOutput output = DebugOutput();

	output.toHUD(FString("Slash"), 2.f, false);
}