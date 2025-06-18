// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PHCommandMonsterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPHCommandMonsterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTHIVE_API IPHCommandMonsterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void PropagateTarget(class APawn* NewTarget) = 0;

	UFUNCTION()
	virtual void SetIsOnAlert(bool InIsOnAlert) = 0;

	UFUNCTION()
	virtual void SetNewDestination(FVector NewDestination) = 0;
};
