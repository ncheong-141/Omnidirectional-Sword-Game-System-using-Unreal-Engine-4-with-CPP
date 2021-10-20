// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockSwordStance.h"

// Debug output file
#include "DebugOutput.h"

BlockSwordStance::~BlockSwordStance()
{
}


// Virtual function override class function implementation
void BlockSwordStance::displayStance() {

	DebugOutput output = DebugOutput();

	output.toHUD(FString("Block Stance"), 2.f, false);
}