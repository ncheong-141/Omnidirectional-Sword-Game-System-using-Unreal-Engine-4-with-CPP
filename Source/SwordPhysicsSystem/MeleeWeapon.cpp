// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set class attributes 
	attackDamage = 1; 
	isSwinging = false; 
	weaponHolder = NULL;

	// Create mesh and set as root compoonent
	mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("mesh"));
	
	// Note, root component is basically meant to be the core of the object and defines how you collide wioththe object
	// UE4 intends rootcomponent to always be referenced to the collision primitive
	RootComponent = mesh;

	// Create proximity box
	proximityBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("proximityBox"));
	
	// Add dynamic/functionality to what happens in this proximity box overlaps another actor
	proximityBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::proximityCheck);
	
	// Attach proximity box to the root component (the mesh here)
	proximityBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}

int AMeleeWeapon::proximityCheck_Implementation(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {

	// Dont hit non-root components 
	if (otherComp != otherActor->GetRootComponent()) {
		return -1; 
	}

	// Dont hit things when conditions arent met
	if (isSwinging && otherActor != (AActor*)weaponHolder && !targetsHit.Contains(otherActor)) {

		otherActor->TakeDamage(attackDamage, FDamageEvent(), NULL, this); 
		targetsHit.Add(otherActor);
	}

	return 0; 
}

void AMeleeWeapon::swing() {

	// Empty the target list for next swing
	targetsHit.Empty(); 

	isSwinging = true;
}

void AMeleeWeapon::rest() {
	targetsHit.Empty(); 
	isSwinging = false; 
}


// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

