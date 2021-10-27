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

private:
	
	/* Class attributes */

	// SwordStance parent class pointer, used for polymorphic referenceing of other Stance classes (concrete class in state pattern)
	SwordStance*		currentStance;

	// Store instances of all stances such that SwordStance* currentStance can interchange which stance it points too
	// This means the stance wont be created/deleted every time it is called. 
	DefaultSwordStance	defaultStance;
	SlashSwordStance	slashStance; 
	BlockSwordStance	blockStance;
	StabSwordStance		stabStance; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

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
	void dodgeLeft(float amount);


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
