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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		AAvatar* animatedAvatar;

	// Curve values (not using TArray for verbosity)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
		float fMovementDistanceCurveCurrentValue;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
		float fMovementDistanceCurveLastFrameValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
		float rMovementDistanceCurveCurrentValue;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
		float rMovementDistanceCurveLastFrameValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
		float upMovementDistanceCurveCurrentValue;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
		float upMovementDistanceCurveLastFrameValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
		bool allowReadingOfAnimationCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
		bool resetReadingOfAnimationCurve;

	// Class functions
	// Called at each animation tick
	virtual void NativeUpdateAnimation(float DeltaSeconds);


	// Post initalisation. Used to initialise memory references 
	// which are not made till after construction
	// Unreal engine apparently makes the link even later after construction between this class and OwningActor
	// So still doesnt work... 
	// void PostInitProperties() override; 

private:

};
