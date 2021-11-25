// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeWeapon.h"
#include "OneHandedSword.generated.h"

/**
 * 
 */
UCLASS()
class SWORDPHYSICSSYSTEM_API AOneHandedSword : public AMeleeWeapon
{
	GENERATED_BODY()

public:

	/* Sword attributes */
	FString name; 
	float mass; 
	float dynamicDamage;

	// Bounding box for weapon collision 
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MeleeWeapon)
		UBoxComponent* proximityBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MeleeWeapon)
		UStaticMeshComponent* mesh;

	/* Constructor */
	AOneHandedSword(const FObjectInitializer& ObjectInitializer);

	/* Initialiser (for initialising other variables) */
	void init(FString name, float mass); 


	// Hit box detection 
	UFUNCTION(BlueprintNativeEvent, Category = Collision)
		void proximityCheck(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	virtual int proximityCheck_Implementation(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);


	/* Class member functions */

	// virtual function override
	virtual void startAttackMotion() override;
	virtual void endAttackMotion() override;

	// Pure virtual override
	// Calculate damage based on sword movement speed and weight
	// This could also depend on location hit
	virtual float calculateDynamicDamage() override;

};
