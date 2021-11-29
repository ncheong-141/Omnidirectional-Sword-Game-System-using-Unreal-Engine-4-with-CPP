// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

// Game classes
#include "SwordFocalPoint.h"

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
	UPROPERTY()
		AAvatar* avatarPtr;

	// Allowable sword directions, set by subclasses during their stance activations
	// E.g. slash can only move 1 direction north or south, west and east.
	// This is calculated in the attack animation notification state tick function
	UPROPERTY()
		AllowableSwordDirectionInformation allowableSwordDirections;
	 
public:

	/* Class attributes */
	int stanceID;							// Used to reference the stance as an integer
	bool applyRotationToSwordFocalPoint;	// Used to tell if input rotation is applied to the sword focal point

	// Constructors
	SwordStance();
	SwordStance(AAvatar* avatar, int stance_ID, bool applyRotationToSFP);

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
	virtual void jump();
	virtual void dodge(); 

	/* Virtual functions which sword stances can override */
	virtual void calculateAllowableSwordDirections(); 

	/* Pure virtual functions which the Sword Stance classes MUST implement */
	virtual void swordStanceActivation() = 0; 
	virtual void swordStanceDeactivation() = 0; 

	// Getters and setters
	const AllowableSwordDirectionInformation* getAllowableSwordDirections();

private:
	/* Internal helper functions for all sword stance classes */
	
};
