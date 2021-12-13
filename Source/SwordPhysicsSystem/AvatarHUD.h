// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AvatarHUD.generated.h"

  
class AAvatar;

// Internal Message class to ouput to HUD
struct HUDMessage {

public:

	// Struct data
	FString message;
	float time;
	FColor colour;

	// Constructor
	HUDMessage(FString message, float time, FColor colourType) {

		// Set the struct attributes 
		this->message = message;
		this->time = time;

		// Set colour
		this->colour = colourType;
	}
};


UCLASS()
class SWORDPHYSICSSYSTEM_API AAvatarHUD : public AHUD
{
	GENERATED_BODY()

protected:

	// The font used to render the text in the HUD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDFont)
		UFont* hudFont;

	// List of messages to display 
	TArray<HUDMessage> messages;

	AAvatar* avatar; 

	// Internal functions
	// Always displayed messages to HUD
	void alwaysDisplayedMessages(std::string& outputStr);

	void drawTemporaryHUDMessages();

	void drawHitpointBar();
public:

	// Add this function tobe able to draw to the HUD
	virtual void DrawHUD() override;

	// FUnction to be able to add a message to display 
	void addHUDMessage(HUDMessage msg);


	// Set avatar ref
	void initialise(AAvatar* avatarRef); 
};
