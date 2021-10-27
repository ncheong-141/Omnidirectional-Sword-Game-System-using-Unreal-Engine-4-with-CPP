// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

/*
*  SwordStance abstract class which is part of the State design pattern. 
*  This class holds the pointer to the Context and defines functionality for concrete classes (sword stance)
*  which is required to be defined. 
*  It also defines some functionality which can be common for all classes 
* 
*  Defining movement and avatar controls in sword stance to further decouple Sword system from 
*  Avatar implementation. This is so the system is easy to add to existing avatars
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
	/* Protected class attributes usable by sub-classes  */
	
	//  Avatar pointer 
	AAvatar* avatarPtr;

	// Flow control variables to control avatar behaviour in specifc situations
	bool cardinalMovementLock;		// Stops WASD movement when true

	// Action characteristics
	float dodgeTime = 1.f; 

	// Sword position of the focal point. Sword root and tip position will be a function of this
	// Sub-stances can alter this variable and use it between stances
	float swordFocalPointPosition[2]; 
	 
public:
	
	// Constructors
	SwordStance() {}
	SwordStance(AAvatar* avatar);

	// Virtual destructor to ensure sub class objects are appopiately deleted
	virtual ~SwordStance();

	/* Functions common to ALL states(SwordStances) (implementation only required once) */

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

	// Action functions
	virtual void dodgeLeft(float amount);


	/* Pure virtual functions which the Sword Stance classes MUST implement */
};
