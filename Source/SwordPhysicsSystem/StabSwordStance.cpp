// Fill out your copyright notice in the Description page of Project Settings.


#include "StabSwordStance.h"

// Debug output file
#include "DebugOutput.h"

StabSwordStance::~StabSwordStance()
{
}

// Virtual function override class function implementation
void StabSwordStance::displayStance() {

	DebugOutput output = DebugOutput();

	output.toHUD(FString("Stab Stance"), 2.f, false);
}