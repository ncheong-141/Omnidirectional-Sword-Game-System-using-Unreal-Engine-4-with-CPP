// Fill out your copyright notice in the Description page of Project Settings.


#include "Avatar.h"

// Debug output for printing to console
#include "DebugOutput.h"

// Unreal engine component classes
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"

// General imports
#include <string>

// Sets default values
AAvatar::AAvatar() {

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Sword stance variables and instanciate objects for referencing
	// Integer values are the stance ID sets
	defaultStance	= DefaultSwordStance(this,0); 
	slashStance		= SlashSwordStance(this,1);
	blockStance		= BlockSwordStance(this,2);
	stabStance		= StabSwordStance(this,3);

	// Set the Avatar sword stance initially to Default.
	AAvatar::setStance(defaultStance);

	// Set the current stance ID variable for reference
	AAvatar::currentStanceID = currentStance->stanceID; 

	// Initialise other variables and control flow
	swordFocalPointPosition_X = 0.f;
	swordFocalPointPosition_Y = 0.f;
	resultantSpeed = 0.f;
	inputVelocity_X = 0.f;
	inputVelocity_Y = 0.f;
	isInAir		= false;
	isInIframe	= false;
	isWalking	= false;

	//avatarMovementVector = GetVelocity();
}


// Called when the game starts or when spawned
void AAvatar::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void AAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	/* Key physics */

	// Velocity update
	FVector avatarVector = GetVelocity();
	resultantSpeed = avatarVector.Size(); 
	inputVelocity_X = GetInputAxisValue(FName("Forward")) + GetInputAxisValue(FName("Back"));
	inputVelocity_Y = GetInputAxisValue(FName("StrafeRight")) + GetInputAxisValue(FName("StrafeLeft"));
	
	// Check if avatar is in the air for physics and animation flow
	isInAir = this->GetCharacterMovement()->IsFalling();
}

// Called to bind functionality to input
void AAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	// Bind the player inputs to the project setting input configuration
	// Keyboard inputs
	PlayerInputComponent->BindAxis("Forward", this, &AAvatar::MoveForward);
	PlayerInputComponent->BindAxis("Back", this, &AAvatar::MoveBack);
	PlayerInputComponent->BindAxis("StrafeLeft", this, &AAvatar::MoveLeft);
	PlayerInputComponent->BindAxis("StrafeRight", this, &AAvatar::MoveRight);

	// Mouse inputs 
	PlayerInputComponent->BindAxis("Yaw", this, &AAvatar::Yaw);
	PlayerInputComponent->BindAxis("Pitch", this, &AAvatar::Pitch);

	// Action inputs 
	PlayerInputComponent->BindAction("debugPrintTest", IE_Pressed, this, &AAvatar::debugMessageOut);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAvatar::jump);

	// Sword stance change input
	PlayerInputComponent->BindAction("DefaultSwordStance", IE_Pressed, this, &AAvatar::switch_DefaultSwordStance);
	PlayerInputComponent->BindAction("SlashSwordStance", IE_Pressed, this, &AAvatar::switch_SlashSwordStance);
	PlayerInputComponent->BindAction("BlockSwordStance", IE_Pressed, this, &AAvatar::switch_BlockSwordStance);
	PlayerInputComponent->BindAction("StabSwordStance", IE_Pressed, this, &AAvatar::switch_StabSwordStance);
}


/* ======================== 
	Class member functions 
======================== */

// Stance functions
SwordStance* AAvatar::getStance() {
	return currentStance; 
}

void AAvatar::setStance(SwordStance& newStance) {
	currentStance = &newStance; 
}

void AAvatar::switch_DefaultSwordStance() {
	currentStance = &defaultStance;
	currentStance->displayStance();
}

void AAvatar::switch_SlashSwordStance() {
	currentStance = &slashStance;
	currentStance->displayStance();
}

void AAvatar::switch_BlockSwordStance() {
	currentStance = &blockStance; 
	currentStance->displayStance(); 
}

void AAvatar::switch_StabSwordStance() {
	currentStance = &stabStance; 
	currentStance->displayStance(); 
}

/* ======================== 
	Player input functions 
	All refer to stance functions such that the stances have full control over Avatar behaviour
======================== */
void AAvatar::MoveForward(float amount) {

	currentStance->MoveForward(amount);
}

void AAvatar::MoveBack(float amount) {

	currentStance->MoveBack(amount);
}

void AAvatar::MoveRight(float amount) {

	currentStance->MoveRight(amount);
}

void AAvatar::MoveLeft(float amount) {

	currentStance->MoveLeft(amount);
}


void AAvatar::Yaw(float amount) {

	currentStance->Yaw(amount);
}

void AAvatar::Pitch(float amount) {

	currentStance->Pitch(amount);
}


/* ========================
	Player Action functions
	All refer to stance functions such that the stances have full control over Avatar behaviour
======================== */

void AAvatar::jump() {

	currentStance->jump();
}

void AAvatar::PostInitializeComponents() {

	Super::PostInitializeComponents(); 

	// Instantiate the melee weapon if a BP was selected in UE4
	if (BPMeleeWeapon) {

		// Instantiate melee weapon
		MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(BPMeleeWeapon, FVector(), FRotator());

		if (MeleeWeapon) {

			// Get refence to the socket
			const USkeletalMeshSocket* socket = GetMesh()->GetSocketByName(FName("RightHandSocket"));
			
			if (socket) {
				// Attach meleeWeapon to socket
				socket->AttachActor(MeleeWeapon, GetMesh());
				MeleeWeapon->weaponHolder = this;
			}
		}
	}
}

void AAvatar::debugMessageOut() {

	// Get a reference to  the controller
	DebugOutput output = DebugOutput();

	std::string strOut = "X,Y,Z input: " + std::to_string(inputVelocity_X) + "," + std::to_string(inputVelocity_Y);


	output.toHUD(FString(strOut.c_str()), 5.f, true);
}

