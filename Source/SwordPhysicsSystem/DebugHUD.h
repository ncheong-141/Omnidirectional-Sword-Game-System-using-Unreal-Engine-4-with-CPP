// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


// General
#include <string>


#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DebugHUD.generated.h"

/**
 * 
 * The DebugHUD class is to display messages to the HUD ingame. 
 * - The messages will display in red if it is a problem.
 * - The message will display in white if it is just information
 * 
 * The DebugMessage class is an internal class used to encapsulate output data
 */

 // Internal Message class to ouput to HUD
struct DebugMessage {

public:

	// Struct data
	FString message;
	float time;
	FColor color;

	// Constructor
	DebugMessage(FString message, float time, bool isProblem) {
		
		// Set the struct attributes 
		this->message = message;
		this->time = time;

		// Set colour based on if the debugmessage is indicating a probkem or not
		if (isProblem) {
			this->color = FColor::Red;
		}
		else {
			this->color = FColor::White;
		}
	}
};

UCLASS()
class SWORDPHYSICSSYSTEM_API ADebugHUD : public AHUD
{
	GENERATED_BODY()
public:

	// The font used to render the text in the HUD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDFont)
	UFont* hudFont;

	// List of messages to display 
	TArray<DebugMessage> messages;

	// FUnction to be able to add a message to display 
	void addDebugMessage(DebugMessage msg);

	// Add this function tobe able to draw to the HUD
	virtual void DrawHUD() override;

private:
	// Internal functions
	// Always displayed messages to HUD
	void alwaysDisplayedMessages(std::string& outputStr);
};


