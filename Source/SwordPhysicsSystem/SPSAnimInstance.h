// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SPSAnimInstance.generated.h"

/**
 * Parent class of the animation blueprint of the project.
 * Purpose of this class is to add control and functionability to the animation handling
 * in the blueprint viewer. 
 */

// Forward declaration to keep implementation in translation units
class AAvatar;

UCLASS(transient, Blueprintable, hideCategories=AnimInstance, BlueprintType)
class SWORDPHYSICSSYSTEM_API USPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	// Contructor and destructor 
	USPSAnimInstance();
	~USPSAnimInstance(); 

	// Class properties usable in blueprints. 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
		AAvatar* animatedAvatar;

	// Reference to current animation
	UAnimMontage* currentAnimMontage; 

	// Class functions
	// Called at each animation tick
	virtual void NativeUpdateAnimation(float DeltaSeconds);


	// Post initalisation. Used to initialise memory references 
	// which are not made till after construction
	// Unreal engine apparently makes the link even later after construction between this class and OwningActor
	// So still doesnt work... 
	//void PostInitProperties() override; 

private:
	// Internal class properties 

};
