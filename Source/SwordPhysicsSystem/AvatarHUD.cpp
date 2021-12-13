// Fill out your copyright notice in the Description page of Project Settings.


#include "AvatarHUD.h"
#include "Avatar.h"


void AAvatarHUD::addHUDMessage(HUDMessage msg) {
	AAvatarHUD::messages.Add(msg);
}

// NEED TO CLEAN THIS UP
void AAvatarHUD::DrawHUD() {

	// Call superclass DrawHUD() function first (for set up?) 
	Super::DrawHUD();

	// Draw temp messages
	drawTemporaryHUDMessages();

	// Draw HP bar
	drawHitpointBar();

}

void AAvatarHUD::drawTemporaryHUDMessages() {

	// Temporary messages
	// Iterate from back to fron through the list, so if we remove an item while iterating it wont be a problem
	for (int i = messages.Num() - 1; i >= 0; i--) {

		// Get the background box the right size for the message
		float outputWidth, outputHeight, pad = 10.f;
		GetTextSize(AAvatarHUD::messages[i].message, outputWidth, outputHeight, hudFont, 1.f);

		// Set message height and position
		float messageH = outputHeight + 2.f * pad;
		float x = 0.f;
		float y = i * messageH;

		// Draw message
		DrawText(AAvatarHUD::messages[i].message, AAvatarHUD::messages[i].colour, x + pad, y + pad, hudFont);

		// Reduce lifetime of message by the time since last frame
		AAvatarHUD::messages[i].time -= GetWorld()->GetDeltaSeconds();

		// If the message time is up, remove it
		if (AAvatarHUD::messages[i].time <= 0) {
			messages.RemoveAt(i);
		}
	}
}

void AAvatarHUD::drawHitpointBar() {

	// Bar parameters
	float barWidth = 200;
	float barHeight = 5; 
	float barPad = 12; 
	float barMargin = 50; 

	// Get % hp
	float percentHP = avatar->getCurrentHP() / avatar->getMaxHP();

	// Get viewprot size
	const FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	// Draw the bar
	DrawRect(FLinearColor(0, 0, 0, 1), viewportSize.X - barWidth - barPad - barMargin, viewportSize.Y - barHeight - barPad - barMargin, barWidth + 2 * barPad, barHeight + 2 * barPad);

	// Draw HP portion
	DrawRect(FLinearColor(1 - percentHP, percentHP, 0, 1), viewportSize.X - barWidth - barMargin, viewportSize.Y - barHeight - barMargin, barWidth*percentHP, barHeight);

}

void AAvatarHUD::initialise(AAvatar* avatarRef) {
	avatar = avatarRef;
}

void AAvatarHUD::alwaysDisplayedMessages(std::string& outputStr) {

	// Mouse position
	APlayerController* PController = GetWorld()->GetFirstPlayerController();

	FVector2D mouse;
	PController->GetMousePosition(mouse.X, mouse.Y);

	outputStr = "Mouse position: " + std::to_string(mouse.X) + "," + std::to_string(mouse.Y);
}