// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "SPSWeaponHolder.h"
#include "NPC.generated.h"

class AMeleeWeapon;
class AAvatar;

UCLASS()
class SWORDPHYSICSSYSTEM_API ANPC : public ACharacter, public ISPSWeaponHolder
{
	GENERATED_BODY()

protected:

	/* Class attributes*/
	// The sphere that the palce can collide with 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
		USphereComponent* proximitySphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
		USphereComponent* attackRangeSphere;

	// NPC properties 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC Properties")
		float currentHitPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Properties")
		float maxHitPoints;

	// This variable is affects animation playrate
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Properties")
		float attackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Properties")
		float movementSpeed;


	// AI control variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Properties")
		bool rotateToAvatar; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Properties")
		bool moveToAvatar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Properties")
		bool attackAvatar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Properties")
		bool blockForAvatar;


	// AI flags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Properties")
		bool avatarInProximity;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC Properties")
		bool attacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC Properties")
		bool inAttackMotion;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC Properties")
		bool moving;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NPC Properties")
		bool isBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NPC Properties")
		bool wasBlocked;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NPC Properties")
		bool hasBeenHit;


	// Attacking
	// Just using a random attack selecter 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
		int numAttacksAvailable; 

	// ID between number of attacks available
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
		int currentAttackID; 


	// Adding weapon to Avatar, assuming a specific mesh is already set
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Properties")
		UClass* BPMeleeWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NPC Properties")
		AMeleeWeapon* MeleeWeapon;

	// Blockign system
	// Using a really basic focal point as dont need the functionality
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC Properties")
		FVector2D npcBasicSwordFocalPoint; 


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Ths runs after the avatars constructor is complete. 
	virtual void PostInitializeComponents() override;
public:	

	// Public variables


	UPROPERTY()
		AAvatar* avatarRef; 

	// Constructor
	// Sets default values for this character's properties
	ANPC(const FObjectInitializer& ObjectInitializer);


	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/* Class member function*/

	// Proximity Sphere
	// Begin overlap
	UFUNCTION(BlueprintNativeEvent, Category = "Collision")
		void proximityCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual int proximityCheck_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// End overlap
	UFUNCTION(BlueprintNativeEvent, Category = "Collision")
		void endProximityCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	virtual int endProximityCheck_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	// Attack Range sphere
	UFUNCTION(BlueprintNativeEvent, Category = "Collision")
		void attackRangeCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual int attackRangeCheck_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// End overlap
	UFUNCTION(BlueprintNativeEvent, Category = "Collision")
		void endAttackRangeCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual int endAttackRangeCheck_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



private:
	/* Internal helper functions */
	// Turn to avatar
	void turnToAvatar();
	
	// Move to avatar
	void moveTowardsAvatar(float deltaSeconds);

	void startAttackingAvatar();

	void putGuardUp();


public:

	/* Getters and setters*/
	bool getHasBeenHit();
	void setHasBeenHit(bool value); 
	bool getIsInAttackMotion(); 
	void setIsInAttackMotion(bool value);


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
