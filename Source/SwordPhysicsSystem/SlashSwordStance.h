// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwordStance.h"

/**
 * 
 */
class SWORDPHYSICSSYSTEM_API SlashSwordStance : public SwordStance
{
public:
	SlashSwordStance();
	~SlashSwordStance();

	virtual void displayStance() override;

};
