// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "MeleeWeapon.generated.h"

// Forward declaration
class AAvatar; 

/*
* Abstract class used to define melee weapon implemenmtation
* Onehanded sword derives from this. Can also give other weapon types similar functionality
*/

UCLASS()
class SWORDPHYSICSSYSTEM_API AMeleeWeapon : public AActor
{
	GENERATED_BODY()

protected:
	/* Abstract class variables */
	// A list of things the meleeweapon already hit this swing
	// Ensures target is only hit once per swing
	TArray<AActor*> targetsHit; 

	// Prevents damage from occuyring when sword is not swinging
	bool inAttackMotion; 

	// Reference to weapon holder to check ensure does not hit themselve
	AAvatar* weaponHolder;

	// UE4 functions 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Constructor: Sets default values for this actor's properties
	AMeleeWeapon(const FObjectInitializer& ObjectInitializer);

	// Bounding box for weapon collision 
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MeleeWeapon)
		UBoxComponent* proximityBox; 

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MeleeWeapon)
		UStaticMeshComponent* mesh; 

	// The amount of damage attacks of this weapon can do (here as assuming all melee weapons do damage)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeleeWeapon)
		float attackDamage;



	/* Class member functions/UE4 functions */
	// Hit box detection 
	UFUNCTION(BlueprintNativeEvent, Category = Collision)
		void proximityCheck(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	virtual int proximityCheck_Implementation(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Virtual functions which sub classes must implement
	virtual void swing();
	virtual void rest();


	// Getters and Setters
	TArray<AActor*> getTargetsHit();

	bool isInAttackMotion();

	AAvatar* getWeaponHolder();
	void setWeaponHolder(AAvatar* avatar);
};
