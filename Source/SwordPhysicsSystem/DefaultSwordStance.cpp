// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultSwordStance.h"
#include "DebugOutput.h"

DefaultSwordStance::DefaultSwordStance(){
}

DefaultSwordStance::~DefaultSwordStance() {

}

void DefaultSwordStance::displayStance() {

	DebugOutput output = DebugOutput(); 
	
	output.toHUD(FString("Default"), 2.f, false);
}