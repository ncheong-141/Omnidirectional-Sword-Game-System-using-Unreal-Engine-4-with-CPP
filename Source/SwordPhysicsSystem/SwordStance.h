// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

/*
*  SwordStance abstract class which is part of the State design pattern. 
*  This class holds the pointer to the Context and defines functionality for concrete classes (sword stance)
*  which is required to be defined. 
*  It also defines some functionality which can be common for all classes 
*/

// Forward declaration such that implementation of the class can be declared in the implementation file (.cpp)
class AAvatar; 

/**
 * 
 */
class SWORDPHYSICSSYSTEM_API SwordStance
{
private:
protected:
	/* Class attributes */
	AAvatar* avatarPtr;

public:
	
	SwordStance() {}

	// Constructor and destructor 
	SwordStance(AAvatar* avatar);
	virtual ~SwordStance();

	/* Functions common to ALL states(SwordStances) (implementation only required once) */
	// Set the Context of the State pattern
	void setContext();

	/* Virtual functions likely common to all states but can be overidden when required */
	virtual void displayStance();

	// Player input functions (WASD)
	virtual void MoveForward(float amount);
	virtual void MoveBack(float amount);
	virtual void MoveLeft(float amount);
	virtual void MoveRight(float amount);

	// Mouse motion 
	virtual void Yaw(float amount);
	virtual void Pitch(float amount);

	/* Pure virtual functions which the Sword Stance classes MUST implement */
};
