// Fill out your copyright notice in the Description page of Project Settings.


#include "Avatar.h"

// Debug output for printing to console
#include "DebugOutput.h"

// Unreal engine component classes
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
AAvatar::AAvatar() {

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Sword stance variables and instanciate objects for referencing
	defaultStance	= DefaultSwordStance(this); 
	slashStance		= SlashSwordStance(this);
	blockStance		= BlockSwordStance(this);
	stabStance		= StabSwordStance(this);

	// Set the Avatar sword stance initially to Default.
	AAvatar::setStance(defaultStance);
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


void AAvatar::PostInitializeComponents() {

	Super::PostInitializeComponents(); 

	// Instantiate the melee weapon if a BP was selected in UE4
	if (BPMeleeWeapon) {

		// Instantiate melee weapon
		MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(BPMeleeWeapon, FVector(), FRotator());

		if (MeleeWeapon) {

			// Get refence to the socket
			const USkeletalMeshSocket* socket = GetMesh()->GetSocketByName(FName("RightHandSocket"));
			
			// Attach meleeWeapon to socket
			socket->AttachActor(MeleeWeapon, GetMesh());
			MeleeWeapon->weaponHolder = this;
		}

	}
}

void AAvatar::debugMessageOut() {

	// Get a reference to  the controller
	DebugOutput output = DebugOutput();

	output.toHUD("Test", 5.f, true);
}

