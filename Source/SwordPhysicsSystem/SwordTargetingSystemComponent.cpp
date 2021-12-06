// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordTargetingSystemComponent.h"
#include "Avatar.h"
#include "NPC.h"

// Sets default values for this component's properties
USwordTargetingSystemComponent::USwordTargetingSystemComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// Istantiate other variables
	currentTarget = nullptr;
	targetsInProximity = TArray<AActor*>();

	// Instantiate sight sphere
	sightProximitySphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Target System Proximity Sphere"));
	sightProximitySphere->SetSphereRadius(500.f);

	// Make USwordTargetingSystemComponent::prox run when this proximity sphere overlaps another actor
	sightProximitySphere->OnComponentBeginOverlap.AddDynamic(this, &USwordTargetingSystemComponent::proximityCheck);
}


// Called when the game starts
void USwordTargetingSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get owner of component and attach sphere to it 
	sightProximitySphere->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
}


// Called every frame
void USwordTargetingSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


UShapeComponent* USwordTargetingSystemComponent::getSightProximityShape() {
	return sightProximitySphere;
}

AActor* USwordTargetingSystemComponent::getCurrentTarget() {
	return currentTarget;
}

TArray<AActor*> USwordTargetingSystemComponent::getPossibleTargets() {

	return targetsInProximity;
}

FRotator USwordTargetingSystemComponent::hardLockOnTarget() {

	// Owning actor and target (just pick first target in set for now)
	AActor* owner = GetOwner(); 
	currentTarget = nullptr;

	// Iterate over set (need a way to select eventually, should only be one target for now) 
	//for (TSet<AActor*>::TIterator it = targetsInProximity.CreateIterator(); it; ++it) {
	//	
	//	if (*(it) != nullptr) {
	//		UE_LOG(LogTemp, Display, TEXT("Set: %s"), *((*(it))->GetName()))
	//		currentTarget = *(it);
	//	}
	//}
	if (targetsInProximity.Num() > 0) {
		currentTarget = targetsInProximity[0];
	}

	if (currentTarget != nullptr) {

		// Get vector to target
		FVector toTarget = currentTarget->GetActorLocation() - owner->GetActorLocation();

		// Get rotation to target and reset pitch as dont want o adjust that?
		FRotator rotationToTarget = toTarget.Rotation();
		rotationToTarget.Pitch = 0;

		return rotationToTarget;
	}
	else {
		return FRotator(0.f); 
	}

}


FRotator USwordTargetingSystemComponent::softLockOnTarget() {
	return FRotator(0.f);
}


/* Internal function */
int USwordTargetingSystemComponent::proximityCheck_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	//This is where our code will fo for what happens when there is an intersection
	UE_LOG(LogTemp, Display, TEXT("In prox check"));

	// Dont hit non root component
	if (OtherComp != OtherActor->GetRootComponent()) {
		return -1;
	}

	// If the overlapped actor is not the player, you should just simply return from the funciton
	if (Cast<ANPC>(OtherActor) != nullptr && !targetsInProximity.Contains(OtherActor)) {

		// Add actor to set
		UE_LOG(LogTemp, Display, TEXT("Adding actor"));
		targetsInProximity.Add(OtherActor);
		UE_LOG(LogTemp, Display, TEXT("Actor in sight sphere %s"), *targetsInProximity[0]->GetFName().ToString());
		UE_LOG(LogTemp, Display, TEXT("targetsInProximity num %d"), targetsInProximity.Num());

	}

	return 0;
}

