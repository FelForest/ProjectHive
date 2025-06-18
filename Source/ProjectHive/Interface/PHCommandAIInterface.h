// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PHCommandAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPHCommandAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTHIVE_API IPHCommandAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void SetIsOnAlert(bool InIsOnAlert) = 0;
	

};
