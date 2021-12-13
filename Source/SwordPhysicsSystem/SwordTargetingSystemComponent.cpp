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
	yawSensitivity = 1.5;
	pitchSensitivity = 0.4;
	lockOnStepSize = 0.2;

	// Instantiate sight sphere
	sightProximitySphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Target System Proximity Sphere"));
	sightProximitySphere->SetSphereRadius(500.f);

	// Make USwordTargetingSystemComponent::prox run when this proximity sphere overlaps another actor
	sightProximitySphere->OnComponentBeginOverlap.AddDynamic(this, &USwordTargetingSystemComponent::sightProxSphereOverlapBegin);
	sightProximitySphere->OnComponentEndOverlap.AddDynamic(this, &USwordTargetingSystemComponent::sightProxSphereOverlapEnd);
}


// Called when the game starts
void USwordTargetingSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get owner of component and attach sphere to it 
	sightProximitySphere->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	sightProximitySphere->SetSphereRadius(500.f);

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

void USwordTargetingSystemComponent::setCurrentTarget() {

	// Iterate over set (need a way to select eventually, should only be one target for now) 
	//for (TSet<AActor*>::TIterator it = targetsInProximity.CreateIterator(); it; ++it) {
	//	
	//	if (*(it) != nullptr) {
	//		UE_LOG(LogTemp, Display, TEXT("Set: %s"), *((*(it))->GetName()))
	//		currentTarget = *(it);
	//	}
	//}
	

	// Just do first element for now, in future will have a much more comprehnsive/efficent method
	if (targetsInProximity.Num() > 0) {
		currentTarget = targetsInProximity[0];
	}
	else {
		// No targets so unlock
		unlockFromTarget();
	}
}

TArray<AActor*> USwordTargetingSystemComponent::getPossibleTargets() {
	return targetsInProximity;
}

void USwordTargetingSystemComponent::lockOnTarget() {

	// Owning actor and target (just pick first target in set for now)
	// Cast to avatar
	AAvatar* avatar = Cast<AAvatar>(GetOwner());

	if (currentTarget != nullptr) {

		// Get vector to target
		FVector toTarget = currentTarget->GetActorLocation() - avatar->GetActorLocation();

		// Get rotation to target and reset pitch as dont want o adjust that?
		FRotator rotationToTarget = toTarget.Rotation();

		// Set the target (more comprehensive in future)
		setCurrentTarget();

		// Diference between current and old rotation 
		FRotator currentRotation = avatar->pController->GetControlRotation();
		
		// There is two rotation differences, anti clockwise and clockwise
		FRotator rotationDifference =  currentRotation - rotationToTarget;
		FRotator clockwiseRotationDifference = FRotator(0.f);
		FRotator antiClockwiseRotationDifference = FRotator(0.f);


		// Proccess Yaw
		if (rotationDifference.Yaw >= 0) {

			// The angle from Current to Target is anti clockwise
			antiClockwiseRotationDifference.Yaw = std::abs(rotationDifference.Yaw); 
			clockwiseRotationDifference.Yaw = 360.f - antiClockwiseRotationDifference.Yaw;
		}
		else if (rotationDifference.Yaw < 0) {

			// The angle from Current to Target is clockwise 
			clockwiseRotationDifference.Yaw = std::abs(rotationDifference.Yaw);
			antiClockwiseRotationDifference.Yaw = 360.f - clockwiseRotationDifference.Yaw;
		}

		// Process pitch
		if (rotationDifference.Pitch >= 0) {

			// The angle from Current to Target is anti clockwise
			antiClockwiseRotationDifference.Pitch = std::abs(rotationDifference.Pitch);
			clockwiseRotationDifference.Pitch = 360.f - antiClockwiseRotationDifference.Pitch;
		}
		else if (rotationDifference.Pitch < 0) {

			// The angle from Current to Target is clockwise 
			clockwiseRotationDifference.Pitch = std::abs(rotationDifference.Pitch);
			antiClockwiseRotationDifference.Pitch = 360.f - clockwiseRotationDifference.Pitch;
		}

		UE_LOG(LogTemp, Display, TEXT("Current Rotation: %f, %f"), currentRotation.Yaw, currentRotation.Pitch)
			UE_LOG(LogTemp, Display, TEXT("rotationToTarget: %f, %f"), rotationToTarget.Yaw, rotationToTarget.Pitch)
			UE_LOG(LogTemp, Display, TEXT("rotationDifference: %f, %f"), rotationDifference.Yaw, rotationDifference.Pitch)
			UE_LOG(LogTemp, Display, TEXT("antiClockwiseRotationDifference: %f, %f"), antiClockwiseRotationDifference.Yaw, antiClockwiseRotationDifference.Pitch)
			UE_LOG(LogTemp, Display, TEXT("clockwiseRotationDiffernce: %f, %f"), clockwiseRotationDifference.Yaw, clockwiseRotationDifference.Pitch);


		// Gradually change to the rotation 
		// Create a new rotation which steps towards this
		FRotator steppedToRotation = FRotator(0.f);

		// Apply yaw 
		// If clockwise, add to current, if anticlockwise, subrract from current
		if (antiClockwiseRotationDifference.Yaw > clockwiseRotationDifference.Yaw) {
			steppedToRotation.Yaw = currentRotation.Yaw + yawSensitivity*(lockOnStepSize*clockwiseRotationDifference.Yaw * GetWorld()->DeltaTimeSeconds);
		}
		else {
			steppedToRotation.Yaw = currentRotation.Yaw - yawSensitivity*(lockOnStepSize*antiClockwiseRotationDifference.Yaw  * GetWorld()->DeltaTimeSeconds);
		}

		// Apply pitch
		if (antiClockwiseRotationDifference.Pitch > clockwiseRotationDifference.Pitch) {
			steppedToRotation.Pitch = currentRotation.Pitch + pitchSensitivity*(lockOnStepSize*clockwiseRotationDifference.Pitch  * GetWorld()->DeltaTimeSeconds);
		}
		else {
			steppedToRotation.Pitch = currentRotation.Pitch - pitchSensitivity*(lockOnStepSize*antiClockwiseRotationDifference.Pitch  * GetWorld()->DeltaTimeSeconds);
		}

		// Set camera rotation to locked on rotation
		avatar->pController->SetControlRotation(steppedToRotation);

	}
}


void USwordTargetingSystemComponent::unlockFromTarget() {
	
	// Reset current target
	currentTarget = nullptr; 
}


/* Internal function */
int USwordTargetingSystemComponent::sightProxSphereOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	//This is where our code will fo for what happens when there is an intersection
	UE_LOG(LogTemp, Display, TEXT("In OverlapBegin check"));

	// Dont hit non root component
	if (OtherComp != OtherActor->GetRootComponent()) {
		return -1;
	}

	// If the overlapped actor is not the player, you should just simply return from the funciton
	if (Cast<ANPC>(OtherActor) != nullptr && !targetsInProximity.Contains(OtherActor)) {

		// Add actor to set
		UE_LOG(LogTemp, Display, TEXT("Adding actor"));
		targetsInProximity.Add(OtherActor);
	}

	return 0;
}


int USwordTargetingSystemComponent::sightProxSphereOverlapEnd_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	//This is where our code will fo for what happens when there is an intersection
	UE_LOG(LogTemp, Display, TEXT("In OverlapEnd check"));

	// Dont hit non root component
	if (OtherComp != OtherActor->GetRootComponent()) {
		return -1;
	}

	// Remove actor from target list
	if (targetsInProximity.Contains(OtherActor)) {
		targetsInProximity.Remove(OtherActor);
	}

	return 0;
}