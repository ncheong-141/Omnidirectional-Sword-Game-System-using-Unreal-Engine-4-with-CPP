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


	// Float curve data of current animation
	const FFloatCurve* ForwardMovementDistanceFloatCurve;
	const FFloatCurve* RightMovementDistanceFloatCurve;
	const FFloatCurve* UpMovementDistanceFloatCurve;

	// Current time and Total animation duration for reference
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
		float currentTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
		float totalAnimationDuration;

	// Flow control parameters altered by animation notification
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
		bool resetReadingOfAnimationCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
		bool animationCurrentlyPlaying;


	/* Class functions */ 

	// Called at each animation tick
	virtual void NativeUpdateAnimation(float DeltaSeconds);

	// Get the movement distance curve values. Called in Anim Notifcation states when appropiate
	void setFloatCurvePointers(UAnimSequenceBase* Animation); 
};
