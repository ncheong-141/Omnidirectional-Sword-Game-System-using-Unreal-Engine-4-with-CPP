// Fill out your copyright notice in the Description page of Project Settings.


#include "Avatar.h"

// Debug output for printing to console
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

// General imports
#include <cmath>
#include <string>

// Sets default values
AAvatar::AAvatar(const FObjectInitializer& ObjectInitializer) {

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

	// Set Spring arm parameters
	cameraSpringArmComponent->TargetArmLength = 400.f;
	cameraZoomMax = 1000.f;
	cameraZoomMin = 100.f; 
	cameraZoomChangeIncrement = 10.f; 

	// Set up camera 
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	avatarMovementComponent = GetCharacterMovement();
	if (avatarMovementComponent) {
		avatarMovementComponent->bOrientRotationToMovement = true;
		avatarMovementComponent->bUseControllerDesiredRotation = false;
	}

	// Avatar stats
	maxHitPoints = 1000; 
	currentHitPoints = maxHitPoints*0.9;
	maxStamina = 100; 
	currentStamina = maxStamina; 
	attackSpeed = 1.f;


	// Set Sword stance variables and instanciate objects for referencing
	// Integer values are the stance ID sets
	defaultStance = DefaultSwordStance(this, 0, false, false);
	slashStance = SlashSwordStance(this, 1, true, true);
	blockStance = BlockSwordStance(this, 2, true, true);
	stabStance = StabSwordStance(this, 3, true, true);
	bodyRotationSlashStance = BodyRotationSlashStance(this, 4, true, true);

	// Set the Avatar sword stance initially to Default.
	AAvatar::setStance(defaultStance);

	// Set the current stance ID variable for reference
	AAvatar::currentStanceID = currentStance->stanceID;

	// Initialise other variables and control flow
	normalisedLocalVelocity = FVector2D(0.f);
	worldVelocity = FVector2D(0.f);
	localVelocity = FVector2D(0.f);
	normalisedLocalVelocityBeforeAction = FVector2D(0.f);
	righthandResultantSpeed = 0.f; 
	isInAir = false;
	isInIframe = false;
	isInDodge = false;
	isInAttackMotion = false;
	isMoving = false;
	wasMovingBeforeAction = false;
	attackMotionStartingSector = 0; 
	dodgeDirection = 0;
	avatarMaxSpeed = this->GetCharacterMovement()->GetMaxSpeed();
	baseYawTurnSpeed = 45.f;
	basePitchTurnSpeed = 5.f;
	inputMovementLocked = false;
	actionAbilityLocked = false; 

	// Generate the viewport sector grid
	viewportGrid = TArray<UViewportSector*>(); 
	generateViewportGrid(); 

	// Instantiate currentViewpointSector 
	currentViewportSector = nullptr;

	// Initiate swordFocalPoint object
	swordFocalPoint = NewObject<USwordFocalPoint>();

	// Instantiate targetting system
	currentTargettingSystem = NewObject<USwordTargetingSystemComponent>(this, TEXT("Target System"));
}


// Called when the game starts or when spawned
void AAvatar::BeginPlay()
{
	Super::BeginPlay();

	// Get reference to player Controller (since should be set up by now, whereas not set up when in constructor)
	// Pointer/references to some commonly used avatar objects
	// Get player controller reference and cast to custom player controller (which is the actual object set in gamemode BP)
	pController = Cast<ASPSPlayerController>(GetWorld()->GetFirstPlayerController());

	// Get anim instance for refernce
	animationInstance = Cast<USPSAnimInstance>(GetMesh()->GetAnimInstance());

	// Get avatar HUD
	avatarHUD = Cast<AAvatarHUD>(pController->GetHUD());
	avatarHUD->initialise(this);

	// Checks 
	if (pController == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("pController is null in %s"), __FUNCTION__);
	}

	if (animationInstance == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("animationInstance is null in %s"), __FUNCTION__);
	}

	if (avatarHUD == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("avatarHUD is null in %s"), __FUNCTION__);
	}
	/* Avatar key systems set up 
	(Required here and in constructor for hot reloads)*/

	// Generate the viewport sector grid
	viewportGrid = TArray<UViewportSector*>();
	generateViewportGrid();

	// Instantiate currentViewpointSector 
	currentViewportSector = nullptr;

	// Initiate swordFocalPoint object
	swordFocalPoint = NewObject<USwordFocalPoint>();

	// Instantiate targetting system
	currentTargettingSystem = NewObject<USwordTargetingSystemComponent>(this, TEXT("Target System"));


}


// Called every frame
void AAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* Key physics */

	// Velocity update
	velocityAndDirectionUpdate();

	// Update focal point (!= 0 => not the default stance)
	// Somehow does not behave well if done in the Yaw or Pitch functions in Slash or Stab stance
	// Change to flag in stance to say if it updates sword focal or not
	swordFocalPoint->update(pController, currentStance->getAllowableSwordDirections(), currentStance->applyRotationToSwordFocalPoint);

	// Set the current viewport sector to where the sword position is currently 
	setCurrentViewportSector();

	// Check if cardinal movement or actions should be locked or not
	inputMovementLockCheck();
	actionAbilityLockCheck();

	// Apply target system
	applyCameraRotationFromTargetSystem(currentStance->lockOnTarget);

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

	// Release "action" inputs for axis input key releases
	PlayerInputComponent->BindAction("MoveForwardRelease", IE_Released, this, &AAvatar::MoveForwardRelease);
	PlayerInputComponent->BindAction("MoveBackRelease", IE_Released, this, &AAvatar::MoveBackRelease);
	PlayerInputComponent->BindAction("StrafeRightRelease", IE_Released, this, &AAvatar::MoveRightRelease);
	PlayerInputComponent->BindAction("StrafeLeftRelease", IE_Released, this, &AAvatar::MoveLeftRelease);


	// Mouse inputs 
	PlayerInputComponent->BindAxis("Yaw", this, &AAvatar::Yaw);
	PlayerInputComponent->BindAxis("Pitch", this, &AAvatar::Pitch);

	// Action inputs 
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAvatar::jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping );
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AAvatar::dodge);


	// Camera change inputs
	PlayerInputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &AAvatar::changeCameraZoomIn);
	PlayerInputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &AAvatar::changeCameraZoomOut);



	// Sword stance change input
	PlayerInputComponent->BindAction("SwordStanceActivation", IE_Pressed, this, &AAvatar::activateSwordStanceActivation);
	PlayerInputComponent->BindAction("SwordStanceActivation", IE_Released, this, &AAvatar::deactivateSwordStanceActivation);

	PlayerInputComponent->BindAction("DefaultSwordStance", IE_Pressed, this, &AAvatar::switch_DefaultSwordStance);
	
	PlayerInputComponent->BindAction("SlashSwordStance", IE_Pressed, this, &AAvatar::switch_SlashSwordStance);
	PlayerInputComponent->BindAction("SlashSwordStance", IE_Released, this, &AAvatar::switch_DefaultSwordStance);
	
	PlayerInputComponent->BindAction("BlockSwordStance", IE_Pressed, this, &AAvatar::switch_BlockSwordStance);
	PlayerInputComponent->BindAction("BlockSwordStance", IE_Released, this, &AAvatar::switch_DefaultSwordStance);
	
	PlayerInputComponent->BindAction("StabSwordStance", IE_Pressed, this, &AAvatar::switch_StabSwordStance);
	PlayerInputComponent->BindAction("StabSwordStance", IE_Released, this, &AAvatar::switch_DefaultSwordStance);

	PlayerInputComponent->BindAction("BodyRotationSlashStance", IE_Pressed, this, &AAvatar::switch_BodyRotationSlashSwordStance);
	PlayerInputComponent->BindAction("BodyRotationSlashStance", IE_Released, this, &AAvatar::switch_DefaultSwordStance);

	//BodyRotationSlashStance
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

	if (!actionAbilityLocked) {

		// Check if any of the current stance keys are currently, if so dont swtich
		for (int i = 0; i < pController->stanceSwitchMappings.Num(); i++) {

			if (pController->IsInputKeyDown(pController->stanceSwitchMappings[i].Key)) {
				return;
			}
		}

		// Call exit function to prepare for exiting stance
		// Sets any flags appropiately for each stance
		currentStance->exitStance();

		// Reset back to default
		bUseControllerRotationYaw = false;
		avatarMovementComponent->bOrientRotationToMovement = true;

		currentStance = &defaultStance;
		currentStanceID = currentStance->stanceID;
		currentStance->displayStance();
	}
}

void AAvatar::switch_SlashSwordStance() {

	if (!actionAbilityLocked) {

		// Call exit function to prepare for exiting stance
		// Sets any flags appropiately for each stance
		currentStance->exitStance();

		// Aim in camera direction
		bUseControllerRotationYaw = true;
		avatarMovementComponent->bOrientRotationToMovement = false;

		currentStance = &slashStance;
		currentStanceID = currentStance->stanceID;
		currentStance->displayStance();
	}
}

void AAvatar::switch_BlockSwordStance() {

	if (!actionAbilityLocked) {
	
		// Call exit function to prepare for exiting stance
		// Sets any flags appropiately for each stance
		currentStance->exitStance();

		// Aim in camera direction
		bUseControllerRotationYaw = true;
		avatarMovementComponent->bOrientRotationToMovement = false;

		// Switch stance
		currentStance = &blockStance;
		currentStanceID = currentStance->stanceID;
		currentStance->displayStance();

		// Set avatar to blocking
		isBlocking = true;
	}
}

void AAvatar::switch_StabSwordStance() {

	if (!actionAbilityLocked) {

		// Call exit function to prepare for exiting stance
		// Sets any flags appropiately for each stance
		currentStance->exitStance();

		// Aim in camera direction
		bUseControllerRotationYaw = true;
		avatarMovementComponent->bOrientRotationToMovement = false;

		currentStance = &stabStance;
		currentStanceID = currentStance->stanceID;
		currentStance->displayStance();
	}
}

void AAvatar::switch_BodyRotationSlashSwordStance() {

	if (!actionAbilityLocked) {

		// Call exit function to prepare for exiting stance
		// Sets any flags appropiately for each stance
		currentStance->exitStance();

		// Aim in camera direction
		bUseControllerRotationYaw = true;
		avatarMovementComponent->bOrientRotationToMovement = false;

		currentStance = &bodyRotationSlashStance;
		currentStanceID = currentStance->stanceID;
		currentStance->displayStance();
	}
}

// Generate the viewport grid datastructure (called in constructor)
void AAvatar::generateViewportGrid() {

	// Generate the grid based on cardinal segments 
	float dx = 1 / axesSegmentNo;
	float dy = 1 / axesSegmentNo; 

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
	for (int j = 0; j < axesSegmentNo; j++) {

		// Get upper and lower Y limits 
		float ylimLower = currentY;
		float ylimUpper = currentY + dy;

		// Round limits to 2 decimal places (want to avoid 0.99999999) 
		ylimLower = (std::round(100 * ylimLower)) / 100.f;
		ylimUpper = (std::round(100 * ylimUpper)) / 100.f;

		// Loop over X direction
		for (int i = 0; i < axesSegmentNo; i++) {

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

	// These funcitons are always called, dont enter body unless amount > 0 
	if (!inputMovementLocked && amount) {

		// Reset avatar to orientate to movemnet when not straging
		//bUseControllerRotationYaw = false;
		//avatarMovementComponent->bOrientRotationToMovement = true;

		currentStance->MoveForward(amount);
	}
}

void AAvatar::MoveForwardRelease() {

}

void AAvatar::MoveBack(float amount) {
	
	// These funcitons are always called, dont enter body unless amount > 0 
	if (!inputMovementLocked && amount) {

		// Change avatar to rotate with camera yaw and no orientate to movement for strafing
		bUseControllerRotationYaw = true;
		avatarMovementComponent->bOrientRotationToMovement = false;

		currentStance->MoveBack(amount);
	}
}

void AAvatar::MoveBackRelease() {
	
	// Reset avatar to orientate to movemnet when not straging
	bUseControllerRotationYaw = false;
	avatarMovementComponent->bOrientRotationToMovement = true;
}

void AAvatar::MoveRight(float amount) {

	// These funcitons are always called, dont enter body unless amount > 0 
	if (!inputMovementLocked && amount) {

		// Change avatar to rotate with camera yaw and no orientate to movement for strafing
		bUseControllerRotationYaw = true;
		avatarMovementComponent->bOrientRotationToMovement = false;

		currentStance->MoveRight(amount);
	}
}

void AAvatar::MoveRightRelease() {

	// Reset avatar to orientate to movemnet when not straging
	bUseControllerRotationYaw = false;
	avatarMovementComponent->bOrientRotationToMovement = true;
}

void AAvatar::MoveLeft(float amount) {

	// These funcitons are always called, dont enter body unless amount > 0 
	if (!inputMovementLocked && amount) {

		// Change avatar to rotate with camera yaw and no orientate to movement for strafing
		bUseControllerRotationYaw = true;
		avatarMovementComponent->bOrientRotationToMovement = false;

		currentStance->MoveLeft(amount);
	}
}

void AAvatar::MoveLeftRelease() {

	// Reset avatar to orientate to movemnet when not straging
	bUseControllerRotationYaw = false;
	avatarMovementComponent->bOrientRotationToMovement = true;

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

void AAvatar::changeCameraZoomIn() {

	UE_LOG(LogTemp, Display, TEXT("%s"), __FUNCTION__);

	// Enforce Min
	if (cameraSpringArmComponent->TargetArmLength - cameraZoomChangeIncrement > cameraZoomMin) {
		cameraSpringArmComponent->TargetArmLength -= cameraZoomChangeIncrement;
	}
}

void AAvatar::changeCameraZoomOut() {
	UE_LOG(LogTemp, Display, TEXT("%s"), __FUNCTION__);
	// Enforce max
	if (cameraSpringArmComponent->TargetArmLength + cameraZoomChangeIncrement < cameraZoomMax ) {
		cameraSpringArmComponent->TargetArmLength += cameraZoomChangeIncrement;
	}
}

void AAvatar::activateSwordStanceActivation() {
	
	if (!actionAbilityLocked) {
		
		// Call stance function (cant activate stances when doing an action)
		currentStance->swordStanceActivation(); 
	}
}

void AAvatar::deactivateSwordStanceActivation() {

	// Call stance function
	currentStance->swordStanceDeactivation(); 
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
void AAvatar::applyAnimMovement() {


	// Check if cat was successful 
	if (animationInstance) {

		// Get current forward vector (Unit vector) 
		FVector avatarFwdVector = GetActorForwardVector();
		FVector avatarRightVector = GetActorRightVector(); 

		// Apply the distance curves to the unit vectors (this gives the displacement)
		avatarFwdVector = avatarFwdVector * (animationInstance-> fMovementDistanceCurveCurrentValue - animationInstance-> fMovementDistanceCurveLastFrameValue);
		avatarRightVector = avatarRightVector* (animationInstance-> rMovementDistanceCurveCurrentValue - animationInstance->rMovementDistanceCurveLastFrameValue);

		// Get current actor location 
		FVector currentLocation = GetActorLocation(); 

		// Calculate new location
		FVector newLocation = currentLocation + avatarFwdVector + avatarRightVector;

		// Set the location
		SetActorLocation(newLocation, true);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to SPSAnimInstance in applyAnimMovement_Dodge()"));
	}
}


void AAvatar::setAttackAnimInformation() {

	// Set right hand speed
	righthandResultantSpeed = animationInstance->righthandMovementCurveCurrentValue; 
}



/* Internal class functions (helpers) */

// Functins to check avatar state and set true/false to these varibles when neccessary
void AAvatar::inputMovementLockCheck() {

	// List of conditions in which would lock cardinal movement (use ||)
	if (isInDodge || isInAir || isInAttackMotion ) {
		inputMovementLocked = true;
	}
	else {
		inputMovementLocked = false;
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
void AAvatar::velocityAndDirectionUpdate() {

	// Calculate the local velocity of the avatar from the World velocity
	FVector avatarWorldVelocity = this->GetVelocity();
	FQuat	avatarWorldRotation = this->GetActorTransform().GetRotation();
	FVector avatarLocalVelocity = avatarWorldRotation.UnrotateVector(avatarWorldVelocity);
	
	// Set world velocity
	worldVelocity.X = avatarWorldVelocity.X;
	worldVelocity.Y = avatarWorldVelocity.Y;

	// Set local/relative velocity
	localVelocity.X = avatarLocalVelocity.X;
	localVelocity.Y = avatarLocalVelocity.Y;
	
	// Calculate the normalised inputed velocity (this is currently wrong)
	avatarMaxSpeed = this->GetCharacterMovement()->GetMaxSpeed();
	normalisedLocalVelocity.X = avatarLocalVelocity.X / avatarMaxSpeed;
	normalisedLocalVelocity.Y = avatarLocalVelocity.Y / avatarMaxSpeed;

	// set movign flag if moving
	if (!localVelocity.IsZero()) {
		isMoving = true;
	}
	else {
		isMoving = false;
	}

	// Set wasmovingBeforeAction 
	// Dont change if user is doing an action 
	if (!isInAttackMotion && !isInDodge) {
		
		if (isMoving == true) {
			wasMovingBeforeAction = true;
			normalisedLocalVelocityBeforeAction = normalisedLocalVelocity;
		}
		else {
			wasMovingBeforeAction = false;
			normalisedLocalVelocityBeforeAction.X = 0.f;
			normalisedLocalVelocityBeforeAction.Y = 0.f;
		}
	}

	// Get actor direciton info
	inputDirection = animationInstance->CalculateDirection(avatarWorldVelocity, this->GetActorRotation());
	turnInput = ACharacter::GetInputAxisValue(FName("Yaw"));

	// Check if avatar is in the air for physics and animation flow
	isInAir = this->GetCharacterMovement()->IsFalling();
}


void AAvatar::applyCameraRotationFromTargetSystem(bool avatarLockedOnTarget) {

	if (avatarLockedOnTarget) {

		currentTargettingSystem->setCurrentTarget();
		currentTargettingSystem->lockOnTarget(); 
	}
}

void AAvatar::debugOutput() {
	/* ------------------- Debug displaying ------------------------ */
	// Show root component of Avatar
	FVector avatarLocation = this->GetActorLocation();
	//DrawDebugSphere(GetWorld(), avatarLocation, 20.f, 20, FColor::Red);
	GEngine->AddOnScreenDebugMessage(1, 100.f, FColor::White, FString::Printf(TEXT("AL X: %f, AL Y: %f, AL Z: %f"), avatarLocation.X, avatarLocation.Y, avatarLocation.Z));

	// Show mouse position
	//GEngine->AddOnScreenDebugMessage(2, 100.f, FColor::White, FString::Printf(TEXT("Mouse X: %f, Mouse Y: %f"), mouse.X, mouse.Y));
	if (swordFocalPoint) {
		GEngine->AddOnScreenDebugMessage(3, 100.f, FColor::White, FString::Printf(TEXT("SP X: %f, SP Y: %f"), swordFocalPoint->position2D.X, swordFocalPoint->position2D.Y));
	}


	// Show right hand socket
	const USkeletalMeshSocket* socket = GetMesh()->GetSocketByName(FName("hand_rSocket"));

	//if (socket) {
	//	DrawDebugSphere(GetWorld(), socket->GetSocketLocation(GetMesh()), 5.f, 20, FColor::Red);
	//}

	// Show avatar flow control variables
	GEngine->AddOnScreenDebugMessage(4, 100.f, FColor::White, FString::Printf(TEXT("isInAir: %d"), isInAir));
	GEngine->AddOnScreenDebugMessage(5, 100.f, FColor::White, FString::Printf(TEXT("isInDodge: %d"), isInDodge));
	GEngine->AddOnScreenDebugMessage(6, 100.f, FColor::White, FString::Printf(TEXT("IML: %d"), inputMovementLocked));
	GEngine->AddOnScreenDebugMessage(7, 100.f, FColor::White, FString::Printf(TEXT("AAL: %d"), actionAbilityLocked));
	GEngine->AddOnScreenDebugMessage(8, 100.f, FColor::White, FString::Printf(TEXT("IAM: %d"), isInAttackMotion));
	GEngine->AddOnScreenDebugMessage(11, 100.f, FColor::White, FString::Printf(TEXT("IsMoving: %d"), isMoving));
	GEngine->AddOnScreenDebugMessage(12, 100.f, FColor::White, FString::Printf(TEXT("wasMoving: %d"), wasMovingBeforeAction));

	// show pitch
	FRotator actorRotation = pController->GetControlRotation(); 

	GEngine->AddOnScreenDebugMessage(9, 100.f, FColor::White, FString::Printf(TEXT("Pitch: %f"), actorRotation.Pitch));
	GEngine->AddOnScreenDebugMessage(10, 100.f, FColor::White, FString::Printf(TEXT("Yaw: %f"), actorRotation.Yaw));
}




/* Targetable Interface functions*/

bool AAvatar::SPSActorIsBlocking() {
	return isBlocking;
}

bool AAvatar::SPSActorWasBlocked() {
	return wasBlocked;
}

void AAvatar::SPSSetActorIsBlocking(bool value) {
	isBlocking = value;
}

void AAvatar::SPSSetActorWasBlocked(bool value) {
	wasBlocked = value; 
}

float AAvatar::SPSActorGetHP() {
	return currentHitPoints; 
}

float AAvatar::SPSActorGetMaxHP() {
	return maxHitPoints;
}

void AAvatar::SPSActorTakeDamage(float amount) {

	// take damage
	currentHitPoints -= amount; 
	
	// Check if dead etc 
}

float AAvatar::getAttackSpeed() {
	return attackSpeed;
}
void AAvatar::setAttackSpeed(float amount) {
	attackSpeed = amount;
}

void AAvatar::stopAttackIfBlocked() {

	// Set blocked back to false
	SPSSetActorWasBlocked(false);

	// Stop attack (which will always be swordStance activation)
	currentStance->swordStanceDeactivation();

	// Set attackspeed back to normal (when have proper animations this wont be needed)
	setAttackSpeed(1.f);
}





/* Getters and setters */
float AAvatar::getCurrentStamina() {
	return currentStamina;
}

float AAvatar::getMaxStamina() {
	return maxStamina;
}

int AAvatar::getCurrentStanceID() {
	return currentStanceID;
}

USwordFocalPoint* const AAvatar::getSwordFocalPoint() {
	return swordFocalPoint;
}

 FVector2D AAvatar::getNormalisedLocalVelocity() {
	 return normalisedLocalVelocity;
 }

 FVector2D AAvatar::getNormalisedLocalVelocityBeforeAction() {
	 return normalisedLocalVelocityBeforeAction;
 }

 FVector2D AAvatar::getWorldVelocity() {
	 return worldVelocity;
 }
 FVector2D AAvatar::getLocalVelocity() {
	 return localVelocity;
 }

float AAvatar::getRighthandResultantSpeed() {
	return righthandResultantSpeed;
}

float AAvatar::getAvatarMaxSpeed() {
	return avatarMaxSpeed;
}

float AAvatar::getBaseYawTurnSpeed() {
	return baseYawTurnSpeed;
}

float AAvatar::getBasePitchTurnSpeed() {
	return basePitchTurnSpeed;
}
bool AAvatar::avatarIsInAir() {
	return isInAir;
}
bool AAvatar::avatarIsInIframe() {
	return isInIframe; 
}
bool AAvatar::avatarIsInDodge() {
	return isInDodge; 
}
bool AAvatar::avatarIsInAttackMotion() {
	return isInAttackMotion; 
}
bool AAvatar::avatarIsMoving() {
	return isMoving;
}
bool AAvatar::avatarWasMovingBeforeAction() {
	return wasMovingBeforeAction;
}

bool AAvatar::isInputMovementLocked() {
	return inputMovementLocked;
}
bool AAvatar::isActionAbilityLocked() {
	return actionAbilityLocked;
}
int AAvatar::getAttackMotionStartingSector() {
	return attackMotionStartingSector;
}
int AAvatar::getDodgeDirection() {
	return dodgeDirection;
}
UViewportSector* const AAvatar::getCurrentViewportSector() {
	return currentViewportSector;
}
TArray<UViewportSector*> const AAvatar::getViewportGrid() {
	return viewportGrid;
}
const float AAvatar::getNumAxesSegments() {
	return axesSegmentNo;
}
AMeleeWeapon* const AAvatar::getMeleeWeapon() {
	return MeleeWeapon;
}

void AAvatar::setCurrentHP(float amount) {

	// Do not exceed max hit points
	if (amount > maxHitPoints) {
		currentHitPoints = maxHitPoints; 
	}
	else {
		currentHitPoints = amount; 
	}
}

void AAvatar::setMaxHP(float amount) {
	maxHitPoints = amount; 
}

void AAvatar::setCurrentStamina(float amount) {
	
	// Do not exceed max stamina points
	if (amount > maxStamina) {
		currentStamina = maxStamina;
	}
	else {
		currentStamina = amount;
	}
}

void AAvatar::setMaxStamina(float amount) {
	maxStamina = amount;
}

void AAvatar::setRighthandResultantSpeed(float amount) {
	righthandResultantSpeed = amount;
}
void AAvatar::setAvatarMaxSpeed(float amount) {
	avatarMaxSpeed = amount;
}
void AAvatar::setBaseYawTurnSpeed(float amount) {
	baseYawTurnSpeed = amount;
}
void AAvatar::setPitchYawTurnSpeed(float amount) {
	basePitchTurnSpeed = amount;
}
void AAvatar::setAvatarIsInAir(bool value) {
	isInAir = value;
}
void AAvatar::setAvatarIsInIframe(bool value) {
	isInIframe = value;
}
void AAvatar::setAvatarIsInDodge(bool value) {
	isInDodge = value;
}
void AAvatar::setAvatarIsInAttackMotion(bool value) {
	isInAttackMotion = value;
}
void AAvatar::setAvatarIsMoving(bool value) {
	isMoving = value;
}
void AAvatar::setAvatarWasMovingBeforeAction(bool value) {
	wasMovingBeforeAction = value;
}

void AAvatar::setAttackMotionStartingSector(int sectorID) {
	attackMotionStartingSector = sectorID; 
}

void AAvatar::setDodgeDirection(int directionID) {
	dodgeDirection = directionID; 
}