// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordStance.h"

// Game class files
#include "Avatar.h"

// Debug file
#include "DebugOutput.h"

// UE4 Global functions
#include "Kismet/GameplayStatics.h"

SwordStance::SwordStance(){}

SwordStance::~SwordStance(){}


void SwordStance::setContext() {

	// Get player avatar
	avatarPtr = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GWorld->GetWorld(), 0));
}

void SwordStance::displayStance() {
	DebugOutput output = DebugOutput();

	output.toHUD(FString("AHH"), 2.f, false);
}