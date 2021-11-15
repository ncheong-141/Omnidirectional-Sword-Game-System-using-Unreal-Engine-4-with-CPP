// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SWORDPHYSICSSYSTEM_API ViewportSector
{
private:
	int		sectorID; 
	FString verboseName; 
	float	xlims[2]; 
	float	ylims[2];

public:

	// Constructor and destructor
	ViewportSector(int sectorID_, FString verboseName_, float xlimLower, float xlimUpper, float ylimLower, float ylimUpper);
	~ViewportSector();


	// Sector operations 
	ViewportSector* checkWithinSector(float swordFocalX, float swordFocalY); 
	

	// Getters and Setters
	float getXlimLower();
	float getXlimUpper(); 
	float getYlimLower(); 
	float getYlimUpper(); 


};
