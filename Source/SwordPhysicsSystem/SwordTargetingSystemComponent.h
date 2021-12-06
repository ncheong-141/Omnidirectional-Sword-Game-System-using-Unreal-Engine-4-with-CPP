// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingSystem.h"
#include "SwordTargetingSystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDPHYSICSSYSTEM_API USwordTargetingSystemComponent : public UActorComponent, pubic ITargetingSystem
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USwordTargetingSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
