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

private:
	float	xlims[2];
	float	ylims[2];
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AvatarProperties)
		int		sectorID;

	// Constructor and destructor
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
