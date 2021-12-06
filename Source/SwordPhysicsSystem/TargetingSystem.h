// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetingSystem.generated.h"

class AAvatar;
class ANPC;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTargetingSystem : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface class used to define behaviour for targetting systems. The avatar should have a reference to this which
can switch in targeting systems when appropiate. This allows for good expandability and flexibility and is not chosen to be an abstract class as some targeting systems may be much differnet. 
Each class which implements this is an actor component. 
 */
class SWORDPHYSICSSYSTEM_API ITargetingSystem
{
	GENERATED_BODY()

public:
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	virtual UShapeComponent* getSightProximityShape() = 0;
	virtual AActor*			getCurrentTarget() = 0;
	virtual void			setCurrentTarget() = 0;
	virtual TArray<AActor*> getPossibleTargets() = 0;
	virtual void			lockOnTarget() = 0;
	virtual void			unlockFromTarget() = 0;


};
