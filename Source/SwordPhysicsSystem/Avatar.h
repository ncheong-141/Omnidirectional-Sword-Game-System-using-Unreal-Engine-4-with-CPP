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
#include "OneHandedSword.h"
#include "SPSPlayerController.h"
#include "SPSAnimInstance.h"
#include "ViewportSector.h"
#include "SwordFocalPoint.h"
#include "Avatar.generated.h"

// Forward declarations to reduce compile time 
class USpringArmComponent;
class UCameraComponent; 
class USkeletalMeshComponent;

UCLASS()
class SWORDPHYSICSSYSTEM_API AAvatar : public ACharacter
{
	GENERATED_BODY()

public:

	/* Game set up avatar variables*/
	// Avatar camera set up. Doing in C++ to give more control over the camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avatar game settings")
		USpringArmComponent* cameraSpringArmComponent; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avatar game settings")
		UCameraComponent* cameraComponent; 


	// Pointers/references to some commonly used Avatar objects to avoid function calls
	// and/or reads from memory
	UPROPERTY()
		ASPSPlayerController* pController;

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

public:

	/* Class attributes */

	// Current stance ID, this variable is used to denote which stance the avatar is in
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float currentStanceID;

	// Sword position focal point. This is the mouse position on the screen where the sword will zone to. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		USwordFocalPoint* swordFocalPoint;


	// Avatar velocity properties (Using primitive types and not FVector as FVector doesnt allow X,Y,Z values to be accessed in BP or i missed it)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float resultantInputVelocity; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float inputVelocity_X; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float inputVelocity_Y;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float worldVelocity_X;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float worldVelocity_Y;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float localVelocity_X;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float localVelocity_Y;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float righthandResultantSpeed;

	// Avatar control properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		float avatarMaxSpeed; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		float baseYawTurnSpeed; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		float basePitchTurnSpeed;


	// Avatar flow control conditions 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		bool isInAir; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		bool isInIframe; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		bool isWalking; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		bool isInDodge; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		bool isInAttackMotion;

	// The sector the attack was initiated from (used to define which blendspace/animation is played)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		int attackMotionStartingSector; 

	// Dodge direction (0 = forward, 1 = backwards, 2 = left, 3 = right
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		int dodgeDirection; 

	// Avatar internal flow control conditions
	bool inputMovementLocked;		// Stops WASD input
	bool actionAbilityLocked;			// Stops any actions such as jump or dodge


	// Sector - This is a objject to indicate where the sword focal point is on the screen
	// It has an ID and holds all sector data and operations which will be used on it
	// 0 - TL, 1 - T, 2 - TR, 3 - L, 4 - M, 5 - R, 6 - BR, 7 - B, 8 - BL
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AvatarProperties)
		UViewportSector* currentViewportSector;
	
	// Discretised viewport 
	UPROPERTY()
		TArray<UViewportSector*> viewportGrid;

	// Number of sectors on X and Y axis
	// Hardcoding segments as 3 atm as to simplify it to top left, top, top right etc. 
	// This can be expanded on in the future however in the generViewportGrid() function
	const float axesSegmentNo = 3; 

	// Adding weapon to Avatar, assuming a specific mesh is already set
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarProperties)
		UClass* BPMeleeWeapon;

	UPROPERTY()
		AMeleeWeapon* MeleeWeapon;


	/* Unreal engine 4 class functions */
	// Constructor: Sets default values for this character's properties
	AAvatar();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Ths runs after the avatars constructor is complete. 
	virtual void PostInitializeComponents() override;


	/* Class member functions */

	// Stance functions
	// Setting the stance
	SwordStance*	getStance();
	void			setStance(SwordStance& newStance);
	void			switch_DefaultSwordStance(); 
	void			switch_SlashSwordStance();
	void			switch_BlockSwordStance();
	void			switch_StabSwordStance(); 

	// Generate the viewport grid datastructure (called in constructor)
	void generateViewportGrid(); 

	// Sets the current viewport sector based on sword focal point position
	void setCurrentViewportSector(); 


	// Player input 
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
	void dodge();
	
	// Used for switching a boolean variable for communicating with animation blueprint
	void activateSwordStanceActivation();
	void deactivateSwordStanceActivation();

	/* Animation information to class function communcicators*/
	// Functions which apply the avatar animation curve values (e.g. for movement/location changes due to animations)
	// Functions use data from the custom Animation instance where the curve data is read at each animation tick
	void applyAnimMovement_Dodge(USPSAnimInstance* avatarAnimInstance);
	void applyAnimMovement_GeneralAttacks();
	void applyAnimMovement_Parry();

private:
	/* Internal class functions */

	// Functins to check avatar state and set true/false to these varibles when neccessary
	void inputMovementLockCheck();
	void actionAbilityLockCheck(); 

	// Helpers
	void velocityUpdate();
	void debugOutput();
};
