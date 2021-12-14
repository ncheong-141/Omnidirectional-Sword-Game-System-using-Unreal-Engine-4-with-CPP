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
#include "BodyRotationSlashStance.h"
#include "MeleeWeapon.h"
#include "OneHandedSword.h"
#include "SPSPlayerController.h"
#include "SPSAnimInstance.h"
#include "ViewportSector.h"
#include "SwordFocalPoint.h"
#include "TargetingSystem.h"
#include "SwordTargetingSystemComponent.h"
#include "AvatarHUD.h"
#include "SPSWeaponHolder.h"
#include "Avatar.generated.h"

// Forward declarations to reduce compile time 
class USpringArmComponent;
class UCameraComponent; 
class USkeletalMeshComponent;

UCLASS()
class SWORDPHYSICSSYSTEM_API AAvatar : public ACharacter, public ISPSWeaponHolder
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Game set up avatar variables*/
	// Avatar camera set up. Doing in C++ to give more control over the camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avatar game settings")
		USpringArmComponent* cameraSpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avatar game settings")
		UCameraComponent* cameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Avatar game settings")
		float cameraZoomChangeIncrement;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Avatar game settings")
		float cameraZoomMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Avatar game settings")
		float cameraZoomMin;

	/* Targeting system */

	// Note, require TScript smart pointer as garbage collector is not interface aware
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avatar game settings")
		TScriptInterface<ITargetingSystem> currentTargettingSystem;

	/* ---------------- Internal class attributes ----------------------- */

	// Avatar stats
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float currentHitPoints; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float maxHitPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float currentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float maxStamina;

	// This variable is affects animation playrate
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float attackSpeed; 

	// SwordStance parent class pointer, used for polymorphic referenceing of other Stance classes (concrete class in state pattern)
	SwordStance*		currentStance;

	// Store instances of all stances such that SwordStance* currentStance can interchange which stance it points too
	// This means the stance wont be created/deleted every time it is called. 
	// Note, each stance has an internal ID variable which is used to reference each stance as an integer and makes it easy to define in UE BPs
	DefaultSwordStance		defaultStance;				// ID = 0
	SlashSwordStance		slashStance;				// ID = 1
	BlockSwordStance		blockStance;				// ID = 2
	StabSwordStance			stabStance;					// ID = 3 
	BodyRotationSlashStance bodyRotationSlashStance;	// ID = 4

	// Current stance ID, this variable is used to denote which stance the avatar is in
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		int currentStanceID;

	// Sword position focal point. This is the mouse position on the screen where the sword will zone to. 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		USwordFocalPoint* swordFocalPoint;

	// Avatar velocity properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		FVector2D normalisedLocalVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		FVector2D normalisedLocalVelocityBeforeAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		FVector2D worldVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		FVector2D localVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float inputDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float righthandResultantSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float turnInput;



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
		bool isInDodge;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		bool isInAttackMotion;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		bool isBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		bool wasBlocked;


	// Moving is always updated in avatar, wasMoving to determine if the isMoving was true
	// and needs to be known before an action which changes speed sets it to false (i.e. after an attack)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		bool isMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		bool wasMovingBeforeAction;



	// Avatar internal flow control conditions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		bool inputMovementLocked;		// Stops WASD input

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		bool actionAbilityLocked;		// Stops any actions such as jump or dodge

	// The sector the attack was initiated from (used to define which blendspace/animation is played)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Avatar Properties")
		int attackMotionStartingSector; 

	// Dodge direction (0 = forward, 1 = backwards, 2 = left, 3 = right
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		int dodgeDirection; 

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AvatarProperties)
		AMeleeWeapon* MeleeWeapon;

public:
	
	/* Public attributes */

	// Pointers/references to some commonly used Avatar objects to avoid function calls
	// and/or reads from memory
	UPROPERTY()
		ASPSPlayerController* pController;
	UPROPERTY()
		USPSAnimInstance* animationInstance;
	UPROPERTY()
		UCharacterMovementComponent* avatarMovementComponent; 
	UPROPERTY()
		AAvatarHUD* avatarHUD; 

	/* Unreal engine 4 class functions */
	// Constructor: Sets default values for this character's properties
	AAvatar(const FObjectInitializer& ObjectInitializer);

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
	void			switch_BodyRotationSlashSwordStance();

	// Player input 
	// All impl. call stance functions such that the stances have full control over Avatar behaviour
	// Functions for input are required in Avatar since UE4 framework requires Avatar function pointers
	void MoveForward(float amount);
	void MoveForwardRelease(); 
	void MoveBack(float amount);
	void MoveBackRelease();
	void MoveLeft(float amount);
	void MoveLeftRelease(); 
	void MoveRight(float amount);
	void MoveRightRelease(); 
	void Yaw(float amount);
	void Pitch(float amount);

	// Player action inputs
	void jump(); 
	void dodge();

	// Player camera change inputs
	void changeCameraZoomIn();
	void changeCameraZoomOut();
	
	// Used for switching a boolean variable for communicating with animation blueprint
	void activateSwordStanceActivation();
	void deactivateSwordStanceActivation();

	/* Animation information to class function communcicators*/
	// Functions which apply the avatar animation curve values (e.g. for movement/location changes due to animations)
	// Functions use data from the custom Animation instance where the curve data is read at each animation tick
	void applyAnimMovement();
	void setAttackAnimInformation();

protected:

	/* Internal class functions */

	// Functins to check avatar state and set true/false to these varibles when neccessary
	void inputMovementLockCheck();
	void actionAbilityLockCheck(); 

	// Helpers
	void velocityAndDirectionUpdate();
	void debugOutput();

	// Generate the viewport grid datastructure (called in constructor)
	void generateViewportGrid();

	// Sets the current viewport sector based on sword focal point position
	void setCurrentViewportSector();

	// Set camera rotation from lock on system
	void applyCameraRotationFromTargetSystem(bool avatarLockedOnTarget);
public:

	/* Targetable Interface functions*/ 
	virtual bool SPSActorIsBlocking() override;
	virtual bool SPSActorWasBlocked() override;
	virtual void SPSSetActorIsBlocking(bool value) override;
	virtual void SPSSetActorWasBlocked(bool value) override;
	virtual float SPSActorGetHP() override; 
	virtual float SPSActorGetMaxHP() override;
	virtual void SPSActorTakeDamage(float amount) override;
	virtual float getAttackSpeed() override;
	virtual void setAttackSpeed(float amount) override;
	virtual void stopAttackIfBlocked() override;

	AMeleeWeapon* const getMeleeWeapon() override;

	/* Getters and setters */

	float getCurrentStamina(); 
	float getMaxStamina(); 
	int getCurrentStanceID();
	USwordFocalPoint* const getSwordFocalPoint();
	FVector2D getNormalisedLocalVelocity();
	FVector2D getNormalisedLocalVelocityBeforeAction();
	FVector2D getWorldVelocity();
	FVector2D getLocalVelocity();
	float getRighthandResultantSpeed();
	float getAvatarMaxSpeed();
	float getBaseYawTurnSpeed();
	float getBasePitchTurnSpeed();
	bool avatarIsInAir();
	bool avatarIsInIframe();
	bool avatarIsInDodge();
	bool avatarIsInAttackMotion();
	bool avatarIsMoving();
	bool avatarWasMovingBeforeAction();
	bool isInputMovementLocked();
	bool isActionAbilityLocked();
	int getAttackMotionStartingSector();
	int getDodgeDirection();
	UViewportSector* const getCurrentViewportSector();
	TArray<UViewportSector*> const getViewportGrid();
	const float getNumAxesSegments();


	void setCurrentHP(float amount); 
	void setMaxHP(float amount);
	void setCurrentStamina(float amount); 
	void setMaxStamina(float amount);
	void setRighthandResultantSpeed(float amount);
	void setAvatarMaxSpeed(float amount);
	void setBaseYawTurnSpeed(float amount);
	void setPitchYawTurnSpeed(float amount);
	void setAvatarIsInAir(bool value);
	void setAvatarIsInIframe(bool value);
	void setAvatarIsInDodge(bool value);
	void setAvatarIsInAttackMotion(bool value);
	void setAvatarIsMoving(bool value);
	void setAvatarWasMovingBeforeAction(bool value);
	void setAttackMotionStartingSector(int sectorID); 
	void setDodgeDirection(int directionID);

}; 
