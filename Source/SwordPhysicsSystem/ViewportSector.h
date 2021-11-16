// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ViewportSector.generated.h"

/**
 * 
 */
UCLASS()
class SWORDPHYSICSSYSTEM_API UViewportSector : public UObject
{
	GENERATED_BODY()

public:
	int		sectorID;
	float	xlims[2];
	float	ylims[2];

	// Constructor and destructor
public:
	UViewportSector(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){};

	// Instantiate object with bvalues
	void populate(int sectorID_, float xlimLower, float xlimUpper, float ylimLower, float ylimUpper);

	// Sector operations 
	bool checkWithinSector(float swordFocalX, float swordFocalY, UViewportSector*& sectorObj);

	// Getters and Setters
	int		getSectorID();
	float	getXlimLower();
	float	getXlimUpper();
	float	getYlimLower();
	float	getYlimUpper();

	// Debug helper
	void printInfoToLog();
};
