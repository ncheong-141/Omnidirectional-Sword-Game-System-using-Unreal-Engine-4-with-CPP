// Fill out your copyright notice in the Description page of Project Settings.


#include "Avatar.h"

// Sets default values
AAvatar::AAvatar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

}


/* Movement functions */


void AAvatar::MoveForward(float amount) {

	// Don't enter the body of this function if the controller is not set up, or amount == 0
	if (Controller && amount) {

		// Get current forward movement
		FVector fwd = GetActorForwardVector();

		// We call add movement to actually move the player by amount in the fwd direction
		AddMovementInput(fwd, amount);
	}
}

void AAvatar::MoveBack(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0
	if (Controller && amount) {

		// Get current forward movement (no back vector) 
		FVector fwd = GetActorForwardVector();

		// Add amount to movement, since it is back it is subtract
		AddMovementInput(fwd, -amount);
	}
}

void AAvatar::MoveRight(float amount) {

	// Dont enter the body ofthis function if the controller is not set up, or amount == 0; 
	if (Controller && amount) {

		// Get current right movement (no left vector) 
		FVector right = GetActorRightVector();

		// Add amount to movement
		AddMovementInput(right, amount);
	}
}

void AAvatar::MoveLeft(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (Controller && amount) {

		// Get current right movemnet
		FVector right = GetActorRightVector();

		// Add amount to movmenet, in this case subract since it is left
		AddMovementInput(right, -amount);
	}
}


void AAvatar::Yaw(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (Controller && amount) {

		// Here 200 is mouse sensitivity (hardcoded for this case), getworld...etc gives you the amount of time that passed between the last frame and this frame
		AddControllerYawInput(200.f * amount * GetWorld()->GetDeltaSeconds());
	}
}

void AAvatar::Pitch(float amount) {

	// Dont enter the body of this function if the controller is not set up, or amount == 0; 
	if (Controller && amount) {

		// Here 200 is mouse sensitivity (hardcoded for this case), getworld...etc gives you the amount of time that passed between the last frame and this frame
		AddControllerPitchInput(200.f * amount * GetWorld()->GetDeltaSeconds());
	}

}
