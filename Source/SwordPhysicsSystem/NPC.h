// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "SPSWeaponHolder.h"
#include "NPC.generated.h"

class AMeleeWeapon;

UCLASS()
class SWORDPHYSICSSYSTEM_API ANPC : public ACharacter, public ISPSWeaponHolder
{
	GENERATED_BODY()

protected:

	/* Class attributes*/
	// The sphere that the palce can collide with 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
		USphereComponent* proximitySphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC Properties")
		float currentHitPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC Properties")
		float maxHitPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NPC Properties")
		bool isBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NPC Properties")
		bool wasBlocked;

	// This variable is affects animation playrate
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float attackSpeed;

	// Adding weapon to Avatar, assuming a specific mesh is already set
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarProperties)
		UClass* BPMeleeWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AvatarProperties)
		AMeleeWeapon* MeleeWeapon;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Ths runs after the avatars constructor is complete. 
	virtual void PostInitializeComponents() override;
public:	

	// Public variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
		bool hasBeenHit;


	// Sets default values for this character's properties
	ANPC(const FObjectInitializer& ObjectInitializer);


	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/* Class member function*/
	// Blueprint native event
	UFUNCTION(BlueprintNativeEvent, Category = "Collision")
		void proximityCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual int proximityCheck_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* Getters and setters*/



	/* Targetable Interface functions*/
	virtual bool SPSActorIsBlocking() override;
	virtual bool SPSActorWasBlocked() override;
	virtual void SPSSetActorIsBlocking(bool value) override;
	virtual void SPSSetActorWasBlocked(bool value) override;
	virtual float SPSActorGetHP() override;
	virtual float SPSActorGetMaxHP() override;
	virtual void SPSActorTakeDamage(float amount) override;
	virtual float getAttackSpeed() override;
	virtual void setAttackSpeed(float amount) override;
	virtual void stopAttackIfBlocked() override;

	AMeleeWeapon* const getMeleeWeapon() override;

};
