// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal engine 4 classes
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Game classes 
#include "SwordStance.h"
#include "DefaultSwordStance.h"
#include "SlashSwordStance.h"
#include "BlockSwordStance.h"
#include "StabSwordStance.h"
#include "MeleeWeapon.h"

#include "Avatar.generated.h"

UCLASS()
class SWORDPHYSICSSYSTEM_API AAvatar : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	/* Internal class attributes */

	// SwordStance parent class pointer, used for polymorphic referenceing of other Stance classes (concrete class in state pattern)
	SwordStance*		currentStance;

	// Store instances of all stances such that SwordStance* currentStance can interchange which stance it points too
	// This means the stance wont be created/deleted every time it is called. 
	// Note, each stance has an internal ID variable which is used to reference each stance as an integer and makes it easy to define in UE BPs
	DefaultSwordStance	defaultStance;		// ID = 0
	SlashSwordStance	slashStance;		// ID = 1
	BlockSwordStance	blockStance;		// ID = 2
	StabSwordStance		stabStance;			// ID = 3 

	// Pointers/references to some commonly used Avatar objects to avoid function calls
	// and/or reads from memory
	//const FTransform* avatarWorldTransform;
	//FVector*	avatarWorldVelocity; 
	//FQuat*		avatarWorldRotation;
	//FVector*	avatarLocalVelocity; 

public:

	/* Class attributes */
	/* Unreal engine class attributes to be used in BPs */


	// Current stance ID, this variable is used to denote which stance the avatar is in
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		float currentStanceID;


	// Sword position focal point. This is the mouse position on the screen where the sword will zone to. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		float swordFocalPointPosition_X; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		float swordFocalPointPosition_Y;


	// Avatar properties (Using primitive types and not FVector as FVector doesnt allow X,Y,Z values to be accessed in BP or i missed it)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
	//	FVector avatarMovementVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		float resultantInputVelocity; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		float inputVelocity_X; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		float inputVelocity_Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		float avatarMaxSpeed; 


	// Avatar flow control conditions 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		bool isInAir; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		bool isInIframe; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		bool isWalking; 


	// Constructor: Sets default values for this character's properties
	AAvatar();
	

	/* Unreal engine 4 class functions */
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	/* Class member functions */
	// Stance setting
	SwordStance*	getStance();
	void			setStance(SwordStance& newStance);
	void			switch_DefaultSwordStance(); 
	void			switch_SlashSwordStance();
	void			switch_BlockSwordStance();
	void			switch_StabSwordStance(); 


	/* Player input */
	// All impl. call stance functions such that the stances have full control over Avatar behaviour
	// Functions for input are required in Avatar since UE4 framework requires Avatar function pointers
	void MoveForward(float amount);
	void MoveBack(float amount);
	void MoveLeft(float amount);
	void MoveRight(float amount);
	void Yaw(float amount);
	void Pitch(float amount);

	// Player action inputs
	void jump(); 


	// Debug HUD test
	void debugMessageOut();


	/*
	Adding weapon to Avatar, assuming a specific mesh is already set
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarProperties)
		UClass* BPMeleeWeapon; 

	AMeleeWeapon* MeleeWeapon; 

	// Ths runs after the avatars constructor is complete. 
	virtual void PostInitializeComponents() override; 



};
