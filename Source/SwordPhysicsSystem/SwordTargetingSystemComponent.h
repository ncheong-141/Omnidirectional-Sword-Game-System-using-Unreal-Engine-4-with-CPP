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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target System")
		float yawSensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target System")
		float pitchSensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target System")
		float lockOnStepSize; 

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
	virtual void	setCurrentTarget() override; 
	virtual TArray<AActor*> getPossibleTargets() override;
	virtual void lockOnTarget() override;
	virtual void	unlockFromTarget() override; 


protected:
	// Internal functions
	UFUNCTION(BlueprintNativeEvent, Category = "Collision")
		void sightProxSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual int sightProxSphereOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, Category = "Collision")
		void sightProxSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual int sightProxSphereOverlapEnd_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
