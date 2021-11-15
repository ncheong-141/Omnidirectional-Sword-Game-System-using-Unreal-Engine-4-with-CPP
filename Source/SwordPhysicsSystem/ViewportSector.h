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
	float	xlims[2]; 
	float	ylims[2];

public:

	// Constructor and destructor
	ViewportSector(int sectorID_, float xlimLower, float xlimUpper, float ylimLower, float ylimUpper);
	~ViewportSector();


	// Sector operations 
	bool checkWithinSector(float swordFocalX, float swordFocalY, ViewportSector*& sectorObj);
	

	// Getters and Setters
	float getXlimLower();
	float getXlimUpper(); 
	float getYlimLower(); 
	float getYlimUpper(); 

	// Debug helper
	void printInfoToLog(); 
};
