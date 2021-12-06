// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingSystem.h"
#include "Components/SphereComponent.h"
#include "SwordTargetingSystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDPHYSICSSYSTEM_API USwordTargetingSystemComponent : public UActorComponent, public ITargetingSystem
{
	GENERATED_BODY()

protected:
	
	// Class attrbutes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target System")
		USphereComponent* sightProximitySphere; 

	// Using set as dont want duplicate
	UPROPERTY()
		AActor* currentTarget;
	UPROPERTY()
		TArray<AActor*> targetsInProximity; 

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	
	// Sets default values for this component's properties
	USwordTargetingSystemComponent(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Interface functions
	virtual UShapeComponent* getSightProximityShape() override;
	virtual AActor* getCurrentTarget() override;
	virtual TArray<AActor*> getPossibleTargets() override;
	virtual FRotator hardLockOnTarget() override;
	virtual FRotator softLockOnTarget() override;


protected:
	// Internal functions
	UFUNCTION(BlueprintNativeEvent, Category = "Collision")
		void proximityCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual int proximityCheck_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
