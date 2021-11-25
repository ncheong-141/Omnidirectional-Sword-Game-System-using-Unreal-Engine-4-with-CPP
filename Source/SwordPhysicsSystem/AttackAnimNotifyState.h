// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackAnimNotifyState.generated.h"

class AAvatar;
class USPSAnimInstance;

/**
 * 
 */
UCLASS()
class SWORDPHYSICSSYSTEM_API UAttackAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	/* Class attributes */
		// Reference to avatar, set up in Begin andFlag to check that avatar cast was succesful for Tick function
	UPROPERTY()
		AAvatar* avatar;

	// Reference to the anim instance and Flag to check that cast was succesful for Tick function
	UPROPERTY()
		USPSAnimInstance* avatarAnimInstance;

public:

	// Overriding functions
	// Begin will set up the animation notify state (set references etc) 
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	// NotifyTick updates whatever game variable needs updated
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	// Wraps up any avatar conditions such is set isInDodge to false etc.
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	/* Internal helper functions*/


};
