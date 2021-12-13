// Fill out your copyright notice in the Description page of Project Settings.
#include "DefaultSwordStance.h"

#include "Avatar.h"
#include "AvatarHUD.h"

// Constructor and destructor implementation
DefaultSwordStance::DefaultSwordStance() {

}

DefaultSwordStance::DefaultSwordStance(AAvatar* avatar, int stance_ID, bool applyRotationToSFP, bool lockOn_Target) : SwordStance(avatar, stance_ID, applyRotationToSFP, lockOn_Target) {

}


DefaultSwordStance::~DefaultSwordStance() {

}

// Virtual function override class function implementation
void DefaultSwordStance::displayStance() {

	avatarPtr->avatarHUD->addHUDMessage(HUDMessage("Default Stance", 2.f, FColor::White));

}

void DefaultSwordStance::swordStanceActivation() {

	// Nothing just yet
}


void DefaultSwordStance::swordStanceDeactivation() {

	// Nothing just yet
}

void DefaultSwordStance::exitStance() {
	// Nothing just yet
}
