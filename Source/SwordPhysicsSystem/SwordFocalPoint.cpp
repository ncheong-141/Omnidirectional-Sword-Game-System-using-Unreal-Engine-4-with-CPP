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
};


void USwordFocalPoint::init(ASPSPlayerController pController) {


}



// Make sure you update viewport size

void USwordFocalPoint::update(ASPSPlayerController* pController, AllowableSwordDirectionInformation allowableSwordDirections) {

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
	// - sword can only move in allowable sword directions. These directions are in format
	// [NorthorSouth, WestorEast] => 1 = North, West, 0 = any, -1 = South, East

	// X Axis
	float SFPX = position2D.X;
	if (activatedPBC_X == false) {
		SFPX = position2D.X + sensitivity*(currentMousePositon.X - oldMousePosition.X);
		//UE_LOG(LogTemp, Display, TEXT("Normal SFPX: %f"), SFPX)
	}
	else {
		// If difference is negative => crossed right boundary
		if (currentMousePositon.X - oldMousePosition.X < 0) {

			// Get offsets between boundaries and mousepositions (this is the true difference distance when combined) 
			//SFPX = position2D.X + sensitivity*((currentMousePositon.X - lowerPBC_X) + (upperPBC_X - oldMousePosition.X));
			SFPX = position2D.X + sensitivity * ((currentMousePositon.X - lowerPBC_X) + (oldMousePosition.X - upperPBC_X));
			//UE_LOG(LogTemp, Display, TEXT("Right boundary SFPX: %f"), SFPX)
		}
		else { // Else difference is positve => crossed left boundary

			//SFPX = position2D.X + sensitivity*((oldMousePosition.X - lowerPBC_X) + (upperPBC_X - currentMousePositon.X));
			SFPX = position2D.X + sensitivity * ((oldMousePosition.X - lowerPBC_X) + (currentMousePositon.X - upperPBC_X));
			//UE_LOG(LogTemp, Display, TEXT("Left boundary SFPX: %f"), SFPX)
		}

		// Deactivate the boundary bool 
		activatedPBC_X = false;
	}
	// Enforce 0 or 1 boundary
	if (SFPX >= 0.f && SFPX <= 1.f) {
		position2D.X = SFPX;
		//UE_LOG(LogTemp, Display, TEXT("New position2D.X: %f"), position2D.X)
	}

	// Y Axis
	float SFPY = position2D.Y;
	if (activatedPBC_Y == false) {
		SFPY = position2D.Y + sensitivity*(currentMousePositon.Y - oldMousePosition.Y);
		//UE_LOG(LogTemp, Display, TEXT("Normal SFPY: %f"), SFPY)
	}
	else {
		// If difference is negative => crossed top boundary
		if (currentMousePositon.Y - oldMousePosition.Y < 0) {

			// Get offsets between boundaries and mousepositions (this is the true difference distance when combined) 
			SFPY = position2D.Y + sensitivity*((currentMousePositon.Y - lowerPBC_Y) + (oldMousePosition.Y - upperPBC_Y));
			//UE_LOG(LogTemp, Display, TEXT("Top boundary SFPY: %f"), SFPY)
		}
		else { // Else difference is positve => crossed bot boundary

			SFPY = position2D.Y + sensitivity*((oldMousePosition.Y - lowerPBC_Y) + (currentMousePositon.Y - upperPBC_Y));
			//UE_LOG(LogTemp, Display, TEXT("Bottom boundary SFPY: %f"), SFPY)
		}

		// Deactivate the boundary bool 
		activatedPBC_Y = false;
	}
	// Enforce 0 or 1 boundary
	if (SFPY >= 0.f && SFPY <= 1.f) {
		position2D.Y = SFPY;
		//UE_LOG(LogTemp, Display, TEXT("position2D.Y: %f"), position2D.Y)
	}
	
	/* Calculate Sword direciton for reference*/
	calculateMouseDirection(pController);

	UE_LOG(LogTemp, Display, TEXT("Direction: %f"), mouseDirection);

	/* Process mouse positon for next check, ensure periodic BCs*/
	// Set periodic boundary conditions on X and Y viewport edges such that mouse will always "move"
	// Note, the activated booleans are used to branch into a different calculation on how to calculate the mouse difference
	applyPeriodicBoundary(pController);
}


float USwordFocalPoint::getSwordDirectionSensitivity() {
	return sensitivity;
}

void USwordFocalPoint::setSwordDirectionSensitivity(float amount) {
	sensitivity = amount;
}

float USwordFocalPoint::getSwordDirection() {
	return mouseDirection;
}


/* Internal helper functions */
void USwordFocalPoint::calculateMouseDirection(ASPSPlayerController* pController) {


	// New vector 
	FVector2D MP_D = FVector2D(0.f);

	// Use pController function as current and old is dependent on periodic boundaries
	pController->GetInputMouseDelta(MP_D.X, MP_D.Y);

	// KEEP IN MIND, 0,0 is top left, 1,1 is bottom right
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