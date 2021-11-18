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
	FVector2D oldMousePosition; 
	FVector2D currentMousePositon;
	FVector2D viewportSize; 
	FVector2D referenceViewportSize; 

	// Required as normalised mouse position is never (1,1) for some reason
	float viewportSizeCheckRelaxationFactor; 

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
