// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugHUD.h"



void ADebugHUD::addDebugMessage(DebugMessage msg) {
	ADebugHUD::messages.Add(msg);
}

void ADebugHUD::DrawHUD() {

	// Call superclass DrawHUD() function first (for set up?) 
	Super::DrawHUD();

	// Iterate from back to fron thru the list, so if we remove an item while iterating it wont be a problem
	for (int i = messages.Num() - 1; i >= 0; i--) {

		// Get the background box the right size for the message
		float outputWidth, outputHeight, pad = 10.f;
		GetTextSize(ADebugHUD::messages[i].message, outputWidth, outputHeight, hudFont, 1.f);

		float messageH = outputHeight + 2.f * pad;
		float x = 0.f;
		float y = i * messageH;

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