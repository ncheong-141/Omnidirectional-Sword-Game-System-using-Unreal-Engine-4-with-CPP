// Fill out your copyright notice in the Description page of Project Settings.


#include "OneHandedSword.h"

// Constructor

AOneHandedSword::AOneHandedSword(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}



void AOneHandedSword::swing() {

	// Empty the target list for next swing
	targetsHit.Empty();
	inAttackMotion = true;
}

void AOneHandedSword::rest() {
	targetsHit.Empty();
	inAttackMotion = false;
}