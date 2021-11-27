// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordFocalPoint.h"

// Game classes
#include "SPSPlayerController.h"

// External
#include<cmath>

USwordFocalPoint::USwordFocalPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

	// Initalise all variables to 0 
	position2D = FVector2D(0.f);
	oldMousePosition = FVector2D(0.f);
	currentMousePositon = FVector2D(0.f);
	oldPosition2D = FVector2D(0.f);

	// Viewport 
	viewportSize = FVector2D(0.f);
	viewportNormalisedBCMaxsize = 0.95;

	// Calculate periodic boundary condition positions 
	lowerPBC_X = (1 - viewportNormalisedBCMaxsize);
	upperPBC_X = viewportNormalisedBCMaxsize;
	lowerPBC_Y = (1 - viewportNormalisedBCMaxsize);
	upperPBC_Y = viewportNormalisedBCMaxsize;
	activatedPBC_X = false;
	activatedPBC_Y = false;
	sensitivity = 0.7f; 
	mouseDirection = 0.f; 

	// Need to move 10% of the viewport in a direciton for it to be dominating
	// This can be changed by the user (future)
	normalisedDistanceTillPredominating = 0.3f;

	// Flags
	dominatingDirection_North = false;
	dominatingDirection_South = false;
	dominatingDirection_West = false;
	dominatingDirection_East = false;
	
	// Index
	cachedDeltaDistances_Index = 0;
};


void USwordFocalPoint::init(ASPSPlayerController pController) {

}



// Make sure you update viewport size

void USwordFocalPoint::update(ASPSPlayerController* pController, const AllowableSwordDirectionInformation* allowableSwordDirections) {

	// Mouse position update and store old
	// Top left is (0,0), bottom right is (1,1)
	oldMousePosition.X = currentMousePositon.X;
	oldMousePosition.Y = currentMousePositon.Y;

	// Old sword focal position for direction calculations
	oldPosition2D.X = position2D.X;
	oldPosition2D.Y = position2D.Y;

	if (pController) {
		pController->GetMousePosition(currentMousePositon.X, currentMousePositon.Y);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("pController in swordFocalPoint::update(..) is null"))
	}

	// Update viewport size (incase changed)
	viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	
	// Normalise the current positons
	currentMousePositon.X /= viewportSize.X;	
	currentMousePositon.Y /= viewportSize.Y;

	/* Calculate sword focal point positions */
	// This is based on differnce between old and new focal positons and also a sensitivity factor
	// Note, 
	// - these cannot exceed 0 or 1 
	// - sword can only move in allowable sword directions. 
	
	// X Axis
	float dx = 0.f; 
	if (activatedPBC_X == false) {
		dx = sensitivity * (currentMousePositon.X - oldMousePosition.X);
	}
	else {
		// If difference is negative => crossed right boundary
		if (currentMousePositon.X - oldMousePosition.X < 0) {

			// Get offsets between boundaries and mousepositions (this is the true difference distance when combined) 
			dx = sensitivity * ((currentMousePositon.X - lowerPBC_X) + (oldMousePosition.X - upperPBC_X));
		}
		else { // Else difference is positve => crossed left boundary

			dx = sensitivity * ((oldMousePosition.X - lowerPBC_X) + (currentMousePositon.X - upperPBC_X));
		}
		// Deactivate the boundary bool 
		activatedPBC_X = false;
	}
	
	// Enforce 0 or 1 boundary
	if (position2D.X + dx >= 0.f && position2D.X + dx <= 1.f) {

		// Check for allowable sword positions 
		// West = -ve, East = +ve
		if (allowableSwordDirections->canMoveEast && dx > 0.f) {
			position2D.X = position2D.X + dx;
		}
		else if (allowableSwordDirections->canMoveWest && dx < 0.f) {
			position2D.X = position2D.X + dx;
		}
		else if (dx != 0.f) {
			UE_LOG(LogTemp, Display, TEXT("Cant move in that direction in X!"));
		}
	}

	// Y Axis
	float dy = 0.f; 
	if (activatedPBC_Y == false) {
		dy = sensitivity * (currentMousePositon.Y - oldMousePosition.Y);
	}
	else {
		// If difference is negative => crossed top boundary
		if (currentMousePositon.Y - oldMousePosition.Y < 0) {

			// Get offsets between boundaries and mousepositions (this is the true difference distance when combined) 
			dy = sensitivity * ((currentMousePositon.Y - lowerPBC_Y) + (oldMousePosition.Y - upperPBC_Y));
		}
		else { // Else difference is positve => crossed bot boundary

			dy = sensitivity * ((oldMousePosition.Y - lowerPBC_Y) + (currentMousePositon.Y - upperPBC_Y));
		}

		// Deactivate the boundary bool 
		activatedPBC_Y = false;
	}
	// Enforce 0 or 1 boundary
	if (position2D.Y + dy >= 0.f && position2D.Y + dy <= 1.f) {

		// Check for allowable sword positions 
		// North -ve, south +ve
		if (allowableSwordDirections->canMoveNorth && dy < 0.f) {
			position2D.Y = position2D.Y + dy;
		}
		else if (allowableSwordDirections->canMoveSouth && dy > 0.f) {
			position2D.Y = position2D.Y + dy;
		}
		else if (dy != 0.f) {
			UE_LOG(LogTemp, Display, TEXT("Cant move in that direction in Y!"));
		}
	}

	/* Calculate Mouse direciton and distances for reference*/
	calculateMouseDirection(pController);

	// Record and calculate mouyse distances and directions when necessary
	// e.g. when in slash stance activation (attack)
	if (recordMouseDeltaDistances) {

		// Set distance cache
		setCachedDistances(dx, dy);

		/* Calculate predom distances */
		calculatePredominatingDistance();
	}

	/* Process mouse positon for next check, ensure periodic BCs*/
	// Set periodic boundary conditions on X and Y viewport edges such that mouse will always "move"
	// Note, the activated booleans are used to branch into a different calculation on how to calculate the mouse difference
	applyPeriodicBoundary(pController);
}


float USwordFocalPoint::getSwordDirectionSensitivity() {
	return sensitivity;
}

float USwordFocalPoint::getSwordDirection() {
	return mouseDirection;
}

bool USwordFocalPoint::isDominantDirectionNorth() {
	return dominatingDirection_North; 
}

bool USwordFocalPoint::isDominantDirectionSouth() {
	return dominatingDirection_South;
}

bool USwordFocalPoint::isDominantDirectionWest() {
	return dominatingDirection_West;
}

bool USwordFocalPoint::isDominantDirectionEast() {
	return dominatingDirection_East;
}

bool USwordFocalPoint::isRecordingMouseDeltaDistances() {
	return recordMouseDeltaDistances; 
}

void USwordFocalPoint::setSwordDirectionSensitivity(float amount) {
	sensitivity = amount;
}

void USwordFocalPoint::setRecordMouseDeltaDistances(bool value) {
	recordMouseDeltaDistances = value;

	// Reset distance cache 
	resetDistanceCache();
}



/* Internal helper functions */
void USwordFocalPoint::calculateMouseDirection(ASPSPlayerController* pController) {

	// New vector 
	FVector2D MP_D = FVector2D(0.f);
	
	// Gets mouse input deltas based on user perspective (i.e. 1,1 in top right corner)
	pController->GetInputMouseDelta(MP_D.X, MP_D.Y);

	// Check if the difference fvector is non 0. Only calculate direction based on a change of focal point
	if (!MP_D.IsNearlyZero()) {

		// Check if X is 0, if so only base direction of X (Y is always opposite for tan (o/a))
		if (MP_D.X != 0.f && MP_D.X != -0.f) {

			float vectorSideRatio = MP_D.Y / MP_D.X;

			// Get quadrant to calculate angle; 
			// Both X and Y position 
			if (MP_D.X > 0 && MP_D.Y > 0) {
				mouseDirection = atan(vectorSideRatio) * 180.f / PI;
			}
			// If X is -ve and Y +ve 
			else if (MP_D.X < 0 && MP_D.Y > 0) {
				mouseDirection = 180.f + (atan(vectorSideRatio) * 180.f / PI);
			}
			// if both X and Y -ve 
			else if (MP_D.X < 0 && MP_D.Y < 0) {
				mouseDirection = 180.f + (atan(vectorSideRatio) * 180.f / PI);
			}
			// if X +ve and Y -ve 
			else if (MP_D.X > 0 && MP_D.Y < 0) {
				mouseDirection = 360.f + (atan(vectorSideRatio) * 180.f / PI);
			}
		}
	}


}

void USwordFocalPoint::setCachedDistances(float dx, float dy) {
	
	/* Cache the delta distances */

	//  Ensure index stays within static array bounds (start overwriting old values)
	if (cachedDeltaDistances_Index + 1 == numCachedDeltaDistance) {
		cachedDeltaDistances_Index = 0; 	// Reset index 
	}

	// Store the direction data, note also times by sensitivity to keep alligned with sword focal point
	cachedDeltaDistances_X[cachedDeltaDistances_Index] = dx;
	cachedDeltaDistances_Y[cachedDeltaDistances_Index] = dy;

	// Increment index
	cachedDeltaDistances_Index++;
}

void USwordFocalPoint::applyPeriodicBoundary(ASPSPlayerController* pController) {

	// Check which boudary has been crossed
	if (currentMousePositon.X < lowerPBC_X) {
		pController->SetMouseLocation(upperPBC_X * viewportSize.X, currentMousePositon.Y * viewportSize.Y);
		activatedPBC_X = true;
	}
	else if (currentMousePositon.X > upperPBC_X) {
		pController->SetMouseLocation(lowerPBC_X * viewportSize.X, currentMousePositon.Y * viewportSize.Y);
		activatedPBC_X = true;
	}

	if (currentMousePositon.Y < lowerPBC_Y) {
		pController->SetMouseLocation(currentMousePositon.X * viewportSize.X, upperPBC_Y * viewportSize.Y);
		activatedPBC_Y = true;
	}
	else if (currentMousePositon.Y > upperPBC_Y) {
		pController->SetMouseLocation(currentMousePositon.X * viewportSize.X, lowerPBC_Y * viewportSize.Y);
		activatedPBC_Y = true;
	}
}

void USwordFocalPoint::calculatePredominatingDistance() {

	// Calculate the dominating distance by adding the cached distances
	// If the cached distances > normalisedDistanceTillPredominating, set as predominating in 
	// the passed data structure 

	// sum distances 
	float sumX = 0.f;
	float sumY = 0.f; 
	for (int i = 0; i < numCachedDeltaDistance; i++) {
		sumX += cachedDeltaDistances_X[i];
		sumY += cachedDeltaDistances_Y[i];
	}

	// Check each iteration if the predominating distance is found
	// note, this is in the unreal coordinate system 0,0 = top left, 1,1 = bottom right
	
	// X dir so West or East
	if (sumX >= normalisedDistanceTillPredominating) {

		// Moving dominantly east 
		dominatingDirection_West = false;
		dominatingDirection_East = true;
	}
	else if (sumX <= -1 * normalisedDistanceTillPredominating) {

		// Moving dominantly west
		dominatingDirection_West = true;
		dominatingDirection_East = false;
	}
	else {
		// Dominant direction not found
		dominatingDirection_West = true;
		dominatingDirection_East = true;
	}


	// Y dir so North or south (recall 1,1) is bottom right corner)
	if (sumY <= -1 * normalisedDistanceTillPredominating) {

		// Moving dominantly North 
		dominatingDirection_South = false;
		dominatingDirection_North = true;
	}
	else if (sumY >= normalisedDistanceTillPredominating) {

		// Moving dominantly south
		dominatingDirection_South = true;
		dominatingDirection_North = false;
	}
	else {
		// Dominant direction not found
		dominatingDirection_South = true;
		dominatingDirection_North = true;
	}

	//UE_LOG(LogTemp, Display, TEXT("dominatingDirection_North: %d"), dominatingDirection_North);
	//UE_LOG(LogTemp, Display, TEXT("dominatingDirection_South: %d"), dominatingDirection_South);
	//UE_LOG(LogTemp, Display, TEXT("dominatingDirection_West: %d"), dominatingDirection_West);
	//UE_LOG(LogTemp, Display, TEXT("dominatingDirection_East: %d"), dominatingDirection_East);
}


void USwordFocalPoint::resetDistanceCache() {

	// Reset to zero 
	for (int i = 0; i < numCachedDeltaDistance; i++) {
		cachedDeltaDistances_X[i] = 0.f;
		cachedDeltaDistances_Y[i] = 0.f; 
	}
}