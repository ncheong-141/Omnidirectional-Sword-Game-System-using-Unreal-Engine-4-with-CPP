// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "MeleeWeapon.generated.h"

// Forward declaration
class AAvatar; 

/*
* Note, Uproprty declaration is to make it completely configurable by blueprints.
*/

UCLASS()
class SWORDPHYSICSSYSTEM_API AMeleeWeapon : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AMeleeWeapon(const FObjectInitializer& ObjectInitializer);

	// The amount of damage attacks of this weapon can do
	// CHANGE LATER 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeleeWeapon)
		float attackDamage; 

	// A list of things the meleeweapon alrady hit this swing
	// Ensures target is only hit once per swing
	TArray<AActor*> targetsHit; 

	// Prevents damage from occuyring when sword is not swinging
	bool isSwinging; 

	// Reference to weapon holder to check ensure does not hit themselve
	AAvatar* weaponHolder;

	// Bounding box for weapon collision 
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MeleeWeapon)
		UBoxComponent* proximityBox; 

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MeleeWeapon)
		UStaticMeshComponent* mesh; 

	UFUNCTION(BlueprintNativeEvent, Category = Collision)
		void proximityCheck(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult); 

	virtual int proximityCheck_Implementation(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	// Lets the weapon know what state the Avatar is in 
	void swing();
	void rest();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
