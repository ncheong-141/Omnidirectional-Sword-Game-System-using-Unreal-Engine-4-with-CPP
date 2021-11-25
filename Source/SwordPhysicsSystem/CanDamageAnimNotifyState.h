// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Animation/AnimNotifies/AnimNotifyState_Trail.h"
#include "CanDamageAnimNotifyState.generated.h"

class AAvatar;
class USPSAnimInstance;
/**
 * 
 */
UCLASS()
class SWORDPHYSICSSYSTEM_API UCanDamageAnimNotifyState : public UAnimNotifyState_Trail//UAnimNotifyState
{
	GENERATED_BODY()

private:
	/* Class attributes */
	// Reference to avatar, set up in Begin andFlag to check that avatar cast was succesful for Tick function
	UPROPERTY()
		AAvatar* avatar;

public:

	// Overriding functions
	// Begin will set up the animation notify state (set references etc) 
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	// NotifyTick updates the canDamage stat constantly
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	// Wraps up any avatar conditions the state introduced
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	/* Internal helper functions*/


};
