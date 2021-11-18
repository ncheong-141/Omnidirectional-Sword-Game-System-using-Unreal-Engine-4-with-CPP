// Fill out your copyright notice in the Description page of Project Settings.


#include "Avatar.h"

// Debug output for printing to console
#include "DebugOutput.h"
#include "DrawDebugHelpers.h"

// Unreal engine component classes
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"

// Game imports
#include "SPSAnimInstance.h"

// General imports
#include <cmath>
#include <string>

// Sets default values
AAvatar::AAvatar() {

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up player camera
	// Instantiate spring arm and attach spring arm to the root component of avatar (the capsule)
	cameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("AvatarSpringArmComponent"));
	cameraSpringArmComponent->SetupAttachment(RootComponent);
	// Can change sprint arm characteristics here (but going to do in blueprints since more visual)

	// Instantiate Camera and attach to spring arm
	cameraComponent = CreateDefaultSubobject <UCameraComponent>(TEXT("AvatarCameraComponent"));
	cameraComponent->SetupAttachment(cameraSpringArmComponent);

	// Set Sword stance variables and instanciate objects for referencing
	// Integer values are the stance ID sets
	defaultStance = DefaultSwordStance(this, 0);
	slashStance = SlashSwordStance(this, 1);
	blockStance = BlockSwordStance(this, 2);
	stabStance = StabSwordStance(this, 3);

	// Set the Avatar sword stance initially to Default.
	AAvatar::setStance(defaultStance);

	// Set the current stance ID variable for reference
	AAvatar::currentStanceID = currentStance->stanceID;

	// Initialise other variables and control flow
	resultantInputVelocity = 0.f;
	inputVelocity_X = 0.f;
	inputVelocity_Y = 0.f;
	worldVelocity_X = 0.f;
	worldVelocity_Y = 0.f;
	localVelocity_X = 0.f;
	localVelocity_Y = 0.f;
	isInAir = false;
	isInIframe = false;
	isWalking = false;
	isInDodge = false;
	isInAttackMotion = false;
	attackMotionStartingSector = 0; 
	dodgeDirection = 0;
	avatarMaxSpeed = this->GetCharacterMovement()->GetMaxSpeed();
	baseYawTurnSpeed = 45.f;
	basePitchTurnSpeed = 5.f;
	cardinalMovementLocked = false;
	actionAbilityLocked = false; 

	// Generate the viewport sector grid
	viewportGrid = TArray<UViewportSector*>(); 
	generateViewportGrid(); 

	// Instantiate currentViewpointSector 
	currentViewportSector = nullptr;

	// Initiate swordFocalPoint object
	swordFocalPoint = NewObject<USwordFocalPoint>();

}


// Called when the game starts or when spawned
void AAvatar::BeginPlay()
{
	Super::BeginPlay();

	// Get reference to player Controller (since should be set up by now, whereas not set up when in constructor)
	// Pointer/references to some commonly used avatar objects
	// Get player controller reference and cast to custom player controller (which is the actual object set in gamemode BP)
	pController = Cast<ASPSPlayerController>(GetWorld()->GetFirstPlayerController());
}


// Called every frame
void AAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* Key physics */

	// Velocity update
	velocityUpdate();

	// Sword focal point update 
	swordFocalPoint->update(pController);

	// Set the current viewport sector to where the sword position is currently 
	setCurrentViewportSector(); 

	// Check if cardinal movement or actions should be locked or not
	cardinalMovementLockCheck();
	actionAbilityLockCheck();

	// Apply movemnets 
	if (isInDodge) {
		applyAnimMovement_Dodge();
	}

	// Check if avatar is in the air for physics and animation flow
	isInAir = this->GetCharacterMovement()->IsFalling();

	// Debug output helper function
	debugOutput(); 
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
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAvatar::jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping );
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AAvatar::dodge);
	PlayerInputComponent->BindAction("ActivateAttack", IE_Pressed, this, &AAvatar::activateAttackMotion);
	PlayerInputComponent->BindAction("ActivateAttack", IE_Released, this, &AAvatar::deactivateAttackMotion);


	// Sword stance change input
	PlayerInputComponent->BindAction("DefaultSwordStance", IE_Pressed, this, &AAvatar::switch_DefaultSwordStance);
	
	PlayerInputComponent->BindAction("SlashSwordStance", IE_Pressed, this, &AAvatar::switch_SlashSwordStance);
	PlayerInputComponent->BindAction("SlashSwordStance", IE_Released, this, &AAvatar::switch_DefaultSwordStance);
	
	PlayerInputComponent->BindAction("BlockSwordStance", IE_Pressed, this, &AAvatar::switch_BlockSwordStance);
	PlayerInputComponent->BindAction("BlockSwordStance", IE_Released, this, &AAvatar::switch_DefaultSwordStance);
	
	PlayerInputComponent->BindAction("StabSwordStance", IE_Pressed, this, &AAvatar::switch_StabSwordStance);
	PlayerInputComponent->BindAction("StabSwordStance", IE_Released, this, &AAvatar::switch_DefaultSwordStance);
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
	currentStanceID = currentStance->stanceID;
}

void AAvatar::switch_DefaultSwordStance() {
	currentStance = &defaultStance;
	currentStanceID = currentStance->stanceID;
	currentStance->displayStance();
}

void AAvatar::switch_SlashSwordStance() {
	currentStance = &slashStance;
	currentStanceID = currentStance->stanceID;
	currentStance->displayStance();
}

void AAvatar::switch_BlockSwordStance() {
	currentStance = &blockStance; 
	currentStanceID = currentStance->stanceID;
	currentStance->displayStance(); 
}

void AAvatar::switch_StabSwordStance() {
	currentStance = &stabStance; 
	currentStanceID = currentStance->stanceID;
	currentStance->displayStance(); 
}

// Generate the viewport grid datastructure (called in constructor)
void AAvatar::generateViewportGrid() {

	// Generate the grid based on cardinal segments 
	float dx = 1 / cardinalSegmentNo;
	float dy = 1 / cardinalSegmentNo; 

	// Generate grid; loop over x and y, starting with y as want this structure: 
	// 0 1 2
	// 3 4 5
	// 6 7 8
	// For IDs. 
	// Hardcoding cardinal segments as 3 atm as to simplify it to top left, top, top right etc. 
	// This can be expanded on in the future however. 
	
	// ID setter variable
	int currentID = 0; 
	float currentX = 0.f;
	float currentY = 0.f; 
	
	// Loop over y direction 
	for (int j = 0; j < cardinalSegmentNo; j++) {

		// Get upper and lower Y limits 
		float ylimLower = currentY;
		float ylimUpper = currentY + dy;

		// Round limits to 2 decimal places (want to avoid 0.99999999) 
		ylimLower = (std::round(100 * ylimLower)) / 100.f;
		ylimUpper = (std::round(100 * ylimUpper)) / 100.f;

		// Loop over X direction
		for (int i = 0; i < cardinalSegmentNo; i++) {

			// Get upper and lower X limits 
			float xlimLower = currentX; 
			float xlimUpper = currentX + dx; 
			
			// Round limits to 2 decimal places (want to avoid 0.99999999) 
			xlimLower = (std::round(100.f * xlimLower)) / 100.f; 
			xlimUpper = (std::round(100.f * xlimUpper)) / 100.f;

			// Create ViewportSector object and put in grid  (emplace creates object already inside array)
			UViewportSector* sector;
			sector = NewObject<UViewportSector>();
			sector->populate(currentID, xlimLower, xlimUpper, ylimLower, ylimUpper);
			viewportGrid.Add(sector); 

			// Increment ID
			currentID++; 

			// Increment current X 
			currentX = currentX + dx;
		}

		// Reset Current X for next iteration
		currentX = 0.f; 

		// Increment currentY
		currentY = currentY + dy;
	}
}

void AAvatar::setCurrentViewportSector() {
	

	// Iterate over grid and check for where focal point is 
	// (Probably a more efficient way of doing this) 
	bool sectorFound = false;
	for (int i = 0; i < viewportGrid.Num(); i++) {

		//it->printInfoToLog();
		// Check if its within this sectors limits, if so, set the current sector pointer to this sector
		if (viewportGrid[i] != nullptr) {
			sectorFound = viewportGrid[i]->checkWithinSector(swordFocalPoint->position2D.X, swordFocalPoint->position2D.Y, currentViewportSector);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Viewportgrid[i] == nullptr"));
		}

		if (sectorFound) {
			return; 
		}
	}

	if (!sectorFound) {
		UE_LOG(LogTemp, Error, TEXT("ERROR: No viewpoint sector found!"));
	}
}


/* ======================== 
	Player input functions 
	All refer to stance functions such that the stances have full control over Avatar behaviour
======================== */
void AAvatar::MoveForward(float amount) {

	if (!cardinalMovementLocked) {
		currentStance->MoveForward(amount);
	}
}

void AAvatar::MoveBack(float amount) {
	if (!cardinalMovementLocked) {
		currentStance->MoveBack(amount);
	}
}

void AAvatar::MoveRight(float amount) {

	if (!cardinalMovementLocked) {
		currentStance->MoveRight(amount);
	}
}

void AAvatar::MoveLeft(float amount) {

	if (!cardinalMovementLocked) {
		currentStance->MoveLeft(amount);
	}
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

	if (!actionAbilityLocked) {
		currentStance->jump();
	}
}

void AAvatar::dodge()
{
	if (!actionAbilityLocked) {
		currentStance->dodge();
	}
}

void AAvatar::activateAttackMotion() {
	isInAttackMotion = true;
	attackMotionStartingSector = currentViewportSector->getSectorID();
}

void AAvatar::deactivateAttackMotion() {
	isInAttackMotion = false;
}

void AAvatar::PostInitializeComponents() {

	Super::PostInitializeComponents(); 

	// Instantiate the melee weapon if a BP was selected in UE4
	if (BPMeleeWeapon) {

		// Instantiate melee weapon (Onehanded sword here, need a check for what is in BPMeleewapon)
		MeleeWeapon = GetWorld()->SpawnActor<AOneHandedSword>(BPMeleeWeapon, FVector(), FRotator());

		// If instantiation was successful, apply it to avatar
		if (MeleeWeapon) {

			// Get refence to the socket
			const USkeletalMeshSocket* socket = GetMesh()->GetSocketByName(FName("hand_rSocket"));
			
			if (socket) {
				// Attach meleeWeapon to socket
				socket->AttachActor(MeleeWeapon, GetMesh());
				MeleeWeapon->setWeaponHolder(this);
			}
		}
	}
}


// Functions which apply the avatar animation curve values (e.g. for movement/location changes due to animations)
// Functions use data from the custom Animation instance where the curve data is read at each animtion tick
void AAvatar::applyAnimMovement_Dodge() {

	// Get anim instance for curve values 
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();

	// Check if animation instance exists 
	if (animInstance != nullptr) {

		// Cast to custom instance (set in UE4 game mode settings such that it creates this subclass on launch)
		USPSAnimInstance* avatarAnimInstance = Cast<USPSAnimInstance>(animInstance); 
		
		// Check if cat was successful 
		if (avatarAnimInstance) {

			UE_LOG(LogTemp, Display, TEXT("----------------------"));

			// Get current forward vector (Unit vector) 
			FVector avatarFwdVector = GetActorForwardVector();
			FVector avatarRightVector = GetActorRightVector(); 
			UE_LOG(LogTemp, Display, TEXT("FV X: %f, FV Y: %f, FV Z: %f"), avatarFwdVector.X, avatarFwdVector.Y, avatarFwdVector.Z);

			UE_LOG(LogTemp, Display, TEXT("Forward distance current: %f"), avatarAnimInstance -> fMovementDistanceCurveCurrentValue);
			UE_LOG(LogTemp, Display, TEXT("Forward distance last frame: %f"), avatarAnimInstance -> fMovementDistanceCurveLastFrameValue);

			// Apply the distance curves to the unit vectors (this gives the displacement)
			avatarFwdVector = avatarFwdVector * (avatarAnimInstance -> fMovementDistanceCurveCurrentValue - avatarAnimInstance -> fMovementDistanceCurveLastFrameValue);
			avatarRightVector = avatarRightVector* (avatarAnimInstance-> rMovementDistanceCurveCurrentValue - avatarAnimInstance->rMovementDistanceCurveLastFrameValue); 
			UE_LOG(LogTemp, Display, TEXT("FV X: %f, FV Y: %f, FV Z: %f"), avatarFwdVector.X, avatarFwdVector.Y, avatarFwdVector.Z);

			// Get current actor location 
			FVector currentLocation = GetActorLocation();
			UE_LOG(LogTemp, Display, TEXT("CLV X: %f, CLV Y: %f, CLV Z: %f"), currentLocation.X, currentLocation.Y, currentLocation.Z);

			// Calculate new location
			FVector newLocation = currentLocation + avatarFwdVector + avatarRightVector;
			UE_LOG(LogTemp, Display, TEXT("NLV X: %f, NLV Y: %f, NLV Z: %f"), newLocation.X, newLocation.Y, newLocation.Z);


			// Set the location
			this->SetActorLocation(newLocation);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Failed to cast to SPSAnimInstance in applyAnimMovement_Dodge()"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve Anim Instance in applyAnimMovement_Dodge()"));
	}
}


void AAvatar::applyAnimMovement_GeneralAttacks() {

}


void AAvatar::applyAnimMovement_Parry() {

}


/* Internal class functions (helpers) */

// Functins to check avatar state and set true/false to these varibles when neccessary
void AAvatar::cardinalMovementLockCheck() {

	// List of conditions in which would lock cardinal movement (use ||)
	if (isInDodge || isInAir) {
		cardinalMovementLocked = true;
	}
	else {
		cardinalMovementLocked = false;
	}

}

void AAvatar::actionAbilityLockCheck() {

	// List of conditions in which would lock actions (use ||)
	if (isInDodge || isInAir) {
		actionAbilityLocked = true;
	}
	else {
		actionAbilityLocked = false;
	}
}


/* Internal class helpers */
void AAvatar::velocityUpdate() {

	// Calculate the local velocity of the avatar from the World velocity
	FVector avatarWorldVelocity = this->GetVelocity();
	FQuat	avatarWorldRotation = this->GetActorTransform().GetRotation();
	FVector avatarLocalVelocity = avatarWorldRotation.UnrotateVector(avatarWorldVelocity);

	// Set world velocity
	worldVelocity_X = avatarWorldVelocity.X;
	worldVelocity_Y = avatarWorldVelocity.Y;

	// Set local/relative velocity
	localVelocity_X = avatarLocalVelocity.X;
	localVelocity_Y = avatarLocalVelocity.Y;

	// Calculate the normalised inputed velocity (this is currently wrong)
	avatarMaxSpeed = this->GetCharacterMovement()->GetMaxSpeed();
	inputVelocity_X = avatarLocalVelocity.X / avatarMaxSpeed;
	inputVelocity_Y = avatarLocalVelocity.Y / avatarMaxSpeed;

	// Calculate resultant velocity (change later)
	resultantInputVelocity = GetVelocity().Size();
}


void AAvatar::debugOutput() {
	/* ------------------- Debug displaying ------------------------ */
	// Show root component of Avatar
	FVector avatarLocation = this->GetActorLocation();
	DrawDebugSphere(GetWorld(), avatarLocation, 20.f, 20, FColor::Red);
	GEngine->AddOnScreenDebugMessage(1, 100.f, FColor::White, FString::Printf(TEXT("AL X: %f, AL Y: %f, AL Z: %f"), avatarLocation.X, avatarLocation.Y, avatarLocation.Z));

	// Show mouse position
	//GEngine->AddOnScreenDebugMessage(2, 100.f, FColor::White, FString::Printf(TEXT("Mouse X: %f, Mouse Y: %f"), mouse.X, mouse.Y));
	GEngine->AddOnScreenDebugMessage(3, 100.f, FColor::White, FString::Printf(TEXT("SP X: %f, SP Y: %f"), swordFocalPoint->position2D.X, swordFocalPoint->position2D.Y));


	// Show right hand socket
	const USkeletalMeshSocket* socket = GetMesh()->GetSocketByName(FName("hand_rSocket"));

	if (socket) {
		DrawDebugSphere(GetWorld(), socket->GetSocketLocation(GetMesh()), 5.f, 20, FColor::Red);
	}

	// Show avatar flow control variables
	GEngine->AddOnScreenDebugMessage(4, 100.f, FColor::White, FString::Printf(TEXT("isInAir: %d"), isInAir));
	GEngine->AddOnScreenDebugMessage(5, 100.f, FColor::White, FString::Printf(TEXT("isInDodge: %d"), isInDodge));
	GEngine->AddOnScreenDebugMessage(6, 100.f, FColor::White, FString::Printf(TEXT("CML: %d"), cardinalMovementLocked));
	GEngine->AddOnScreenDebugMessage(7, 100.f, FColor::White, FString::Printf(TEXT("AAL: %d"), actionAbilityLocked));
	GEngine->AddOnScreenDebugMessage(8, 100.f, FColor::White, FString::Printf(TEXT("IAM: %d"), isInAttackMotion));

	// show pitch
	FRotator actorRotation = pController->GetControlRotation(); 

	GEngine->AddOnScreenDebugMessage(9, 100.f, FColor::White, FString::Printf(TEXT("Pitch: %f"), actorRotation.Pitch));
	GEngine->AddOnScreenDebugMessage(10, 100.f, FColor::White, FString::Printf(TEXT("Yaw: %f"), actorRotation.Yaw));


}
