// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "NPC.generated.h"

UCLASS()
class SWORDPHYSICSSYSTEM_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:

	/* Class attributes*/
	// The sphere that the palce can collide with 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
		USphereComponent* proximitySphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
		bool hasBeenHit; 

	// Sets default values for this character's properties
	ANPC(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Class member function*/

	// Blueprint native event
	UFUNCTION(BlueprintNativeEvent, Category = "Collision")
		void proximityCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual int proximityCheck_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
