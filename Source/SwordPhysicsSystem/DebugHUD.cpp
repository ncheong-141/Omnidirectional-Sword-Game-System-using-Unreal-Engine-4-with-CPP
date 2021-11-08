// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugHUD.h"



void ADebugHUD::addDebugMessage(DebugMessage msg) {
	ADebugHUD::messages.Add(msg);
}

// NEED TO CLEAN THIS UP
void ADebugHUD::DrawHUD() {

	// Call superclass DrawHUD() function first (for set up?) 
	Super::DrawHUD();

	// Add always display messages
	//std::string outputStr; 
	//ADebugHUD::alwaysDisplayedMessages(outputStr);

	// Always display message size and padding
	float outputWidth, outputHeight, pad = 10.f;
	//GetTextSize(FString(outputStr.c_str()), outputWidth, outputHeight, hudFont, 1.f);
	float messageH = outputHeight + 2.f * pad;
	float x = 0.f;
	float y = messageH;

	//DrawText(FString(outputStr.c_str()), FColor::White, x + pad, y + pad, hudFont);

	// Temporary messages
	// Iterate from back to fron through the list, so if we remove an item while iterating it wont be a problem
	for (int i = messages.Num() - 1; i >= 0; i--) {

		// Get the background box the right size for the message
		outputWidth, outputHeight, pad = 10.f;
		GetTextSize(ADebugHUD::messages[i].message, outputWidth, outputHeight, hudFont, 1.f);

		messageH = outputHeight + 2.f * pad;
		x = 0.f;
		y = i * messageH;

		// Draw message
		DrawText(ADebugHUD::messages[i].message, ADebugHUD::messages[i].color, x + pad, y + pad, hudFont);

		// Reduce lifetime of message by the time since last frame
		ADebugHUD::messages[i].time -= GetWorld()->GetDeltaSeconds();

		// If the message time is up, remove it
		if (ADebugHUD::messages[i].time <= 0) {
			messages.RemoveAt(i);
		}

	}
}

void ADebugHUD::alwaysDisplayedMessages(std::string& outputStr) {

	// Mouse position
	APlayerController* PController = GetWorld()->GetFirstPlayerController(); 

	FVector2D mouse;
	PController->GetMousePosition(mouse.X, mouse.Y);

	outputStr = "Mouse position: " + std::to_string(mouse.X) + "," + std::to_string(mouse.Y);
}