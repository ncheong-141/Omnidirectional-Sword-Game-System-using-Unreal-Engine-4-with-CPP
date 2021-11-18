// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SwordFocalPoint.generated.h"

/**
 * 
 */

class ASPSPlayerController; 

UCLASS()
class SWORDPHYSICSSYSTEM_API USwordFocalPoint : public UObject
{
	GENERATED_BODY()

private:

	// Class internal attributes
	UPROPERTY()
		FVector2D oldMousePosition; 
	UPROPERTY()
		FVector2D currentMousePositon;
	UPROPERTY()
		FVector2D viewportSize; 

	// Required as normalised mouse position is never (1,1) for some reason
	// The scaler is the proportion of the total viewport which the reference is scaled too
	float viewportNormalisedBCMaxsize; 
	float lowerPBC_X;
	float upperPBC_X;
	float lowerPBC_Y;
	float upperPBC_Y;
	bool activatedPBC_X; 
	bool activatedPBC_Y; 

	float sensitivity; 

public:

	// Class external attributes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SFP Properties")
		FVector2D position2D;


	// Constructor 
	USwordFocalPoint(const FObjectInitializer& ObjectInitializer);

	// Initialise (need to learn how to construct UOBjects with parameters..)
	void init(ASPSPlayerController pController); 

	// Class member functions
	void update(ASPSPlayerController* pController);
	
};
