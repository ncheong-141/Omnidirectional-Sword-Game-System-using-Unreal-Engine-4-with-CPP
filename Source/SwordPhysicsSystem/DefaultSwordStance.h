// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwordStance.h"

/**
 * 
 */
class SWORDPHYSICSSYSTEM_API DefaultSwordStance : public SwordStance
{
public:
	DefaultSwordStance();
	~DefaultSwordStance(); 

	virtual void displayStance() override;
};
