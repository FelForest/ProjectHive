// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PHMonsterAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPHMonsterAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTHIVE_API IPHMonsterAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void RunAI() = 0;

	UFUNCTION()
	virtual void StopAI() = 0;

	UFUNCTION()
	virtual void SetIsCombat(bool NewState) = 0;

	UFUNCTION()
	virtual bool GetIsCombat() const = 0;
};
