// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashSwordStance.h"
#include "DebugOutput.h"


SlashSwordStance::SlashSwordStance()
{
}

SlashSwordStance::~SlashSwordStance()
{
}

void SlashSwordStance::displayStance() {

	DebugOutput output = DebugOutput();

	output.toHUD(FString("Slash"), 2.f, false);
}