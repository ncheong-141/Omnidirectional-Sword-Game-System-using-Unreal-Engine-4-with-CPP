// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Forward declaration such that implementation of the c;ass can be declared in the implementation file (.cpp)
class AAvatar; 

/**
 * 
 */
class SWORDPHYSICSSYSTEM_API SwordStance
{
public:

	// Constructor and destructor 
	SwordStance();
	~SwordStance();

	AAvatar* avatarPtr;

	/* Functions common to ALL states(SwordStances) (implementation only required once) */
	// Set the Context of the State pattern

	/* Virtual functions likely common to all states but can be overidden when required*/
	virtual void setContext();
	virtual void displayStance();
};
