// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SwordFocalPoint.generated.h"

/**
 * 
 */

// Forward declarations
class ASPSPlayerController; 

// Struct for containing information on the allowable directions
struct AllowableSwordDirectionInformation {

	// Initialise with true 
	bool canMoveNorth = true; 
	bool canMoveSouth = true;
	bool canMoveWest = true;
	bool canMoveEast = true;
};

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
		FVector2D oldPosition2D; 
	UPROPERTY()
		FVector2D viewportSize; 

	// Required as normalised mouse position is never (1,1) for some reason
	// The scaler is the proportion of the total viewport which the reference is scaled too
	float viewportNormalisedBCMaxsize; 
	float lowerPBC_X;
	float upperPBC_X;
	float lowerPBC_Y;
	float upperPBC_Y;

	// Flags to tell if mouse has crossed the periodic boundary
	bool activatedPBC_X; 
	bool activatedPBC_Y; 

	// Arbitrary weight for user mouse motion 
	float sensitivity; 

	/* Direction and distance variables */

	// Mouse direction currently
	float mouseDirection;

	// Normalised distance till dominating: 
	float normalisedDistanceTillPredominating; 
	
	// Booleans for flagging which distance is currently dominating
	bool dominatingDirection_North;
	bool dominatingDirection_South;
	bool dominatingDirection_West;
	bool dominatingDirection_East;

	// Distance storage
	// The number of cached normalised distances reaching predom threshold
	// is frame dependent/sensitificty, this is bad but if you make it large enough, it shouldnt matter
	// A better solution can be found in the future
	static const int	numCachedDeltaDistance = 50; 
	float				cachedDeltaDistances_X[numCachedDeltaDistance] = { 0 };
	float				cachedDeltaDistances_Y[numCachedDeltaDistance] = { 0 };
	int					cachedDeltaDistances_Index;

	// Should only record when attacking (for now, maybe other features used this)
	bool				recordMouseDeltaDistances; 

	
public:

	// Class external attributes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SFP Properties")
		FVector2D position2D;


	// Constructor 
	USwordFocalPoint(const FObjectInitializer& ObjectInitializer);

	// Initialise (need to learn how to construct UOBjects with parameters..)
	void init(ASPSPlayerController pController); 

	/* Class member functions */

	// Called during avatar tick/per frame. Updates the focal point and any relevant
	// SF stats such as direction, dominant direction etc
	void update(ASPSPlayerController* pController, const AllowableSwordDirectionInformation* allowableSwordDirections);
	

	// Getters and setters
	float getSwordDirectionSensitivity();
	float getSwordDirection(); 
	bool isDominantDirectionNorth();
	bool isDominantDirectionSouth(); 
	bool isDominantDirectionWest(); 
	bool isDominantDirectionEast(); 
	bool isRecordingMouseDeltaDistances();

	void setSwordDirectionSensitivity(float amount);
	void setRecordMouseDeltaDistances(bool value); 



private: 
	/* Internal helper functions */
	void calculateMouseDirection(ASPSPlayerController* pController);
	void setCachedDistances(float dx, float dy);
	void applyPeriodicBoundary(ASPSPlayerController* pController);
	void calculatePredominatingDistance();
	void resetDistanceCache(); 
};
