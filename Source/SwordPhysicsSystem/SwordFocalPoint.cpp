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
	viewportSizeCheckRelaxationFactor = 0.9;
	referenceViewportSize = viewportSizeCheckRelaxationFactor * viewportSize;
};


void USwordFocalPoint::init(ASPSPlayerController pController) {


}



// Make sure you update viewport size

void USwordFocalPoint::update(ASPSPlayerController* pController) {

	// Mouse position update and store old
	// Top left is (0,0), bottom right is (1,1)
	oldMousePosition.X = currentMousePositon.X;
	oldMousePosition.Y = currentMousePositon.Y; 
	pController->GetMousePosition(currentMousePositon.X, currentMousePositon.Y);


	if (GEngine) {
		
		// Update viewport size (incase changed)
		viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

		//// Set periodic boundary conditions such that mouse will always "move"
		//if (currentMousePositon.X <= 0) {
		//	pController->SetMouseLocation(viewportSize.X, currentMousePositon.Y);
		//}
		//else if (currentMousePositon.X >= viewportSizeCheckRelaxationFactor * viewportSize.X) {
		//	pController->SetMouseLocation(0, currentMousePositon.Y);
		//}

		//if (currentMousePositon.Y <= 0) {
		//	pController->SetMouseLocation(currentMousePositon.X, viewportSize.Y);
		//}
		//else if (currentMousePositon.Y >= viewportSizeCheckRelaxationFactor * viewportSize.Y) {
		//	pController->SetMouseLocation(currentMousePositon.X, 0);
		//}


		position2D.X = currentMousePositon.X / viewportSize.X;
		position2D.Y = currentMousePositon.Y / viewportSize.Y;
	}
}
