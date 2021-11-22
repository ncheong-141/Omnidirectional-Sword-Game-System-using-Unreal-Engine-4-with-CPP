// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordFocalPoint.h"

// Game classes
#include "SPSPlayerController.h"


USwordFocalPoint::USwordFocalPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

	// Initalise all variables to 0 
	position2D = FVector2D(0.f);
	oldMousePosition = FVector2D(0.f);
	currentMousePositon = FVector2D(0.f);

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
};


void USwordFocalPoint::init(ASPSPlayerController pController) {


}



// Make sure you update viewport size

void USwordFocalPoint::update(ASPSPlayerController* pController) {

	// Mouse position update and store old
	// Top left is (0,0), bottom right is (1,1)
	oldMousePosition.X = currentMousePositon.X;
	oldMousePosition.Y = currentMousePositon.Y;
	
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
	// Note, these cannot exceed 0 or 1 
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
	
	/* Process mouse positon for next check, ensure periodic BCs*/
	// Set periodic boundary conditions on X and Y viewport edges such that mouse will always "move"
	// Note, the activated booleans are used to branch into a different calculation on how to calculate the mouse difference
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
