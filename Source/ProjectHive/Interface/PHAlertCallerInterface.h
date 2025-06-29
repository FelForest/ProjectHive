﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PHAlertCallerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPHAlertCallerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

DECLARE_DELEGATE(FMonsterAlertFinished);

class PROJECTHIVE_API IPHAlertCallerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(Category = "Alert")
	virtual void CallAlertTarget() = 0;

	UFUNCTION(Category = "Alert")
	virtual void CallAlertDestination() = 0;

	// 오버로드 하려면 리플리케이션 사용 못함 
	virtual void CallAlertTargetBegin(APawn* NewTarget) = 0;
	virtual void CallAlertDestinationBegin(APawn* NewTarget) = 0;

	virtual void CallAlertTargetEnd() = 0;
	virtual void CallAlertDestinationEnd(class UAnimMontage* Montage, bool bInterrupted) = 0;

	virtual bool IsAlerting() = 0;
	virtual void SetIsAlerting(bool InIsAlerting) = 0;

	virtual void SetMonsterAlertDelegate(const FMonsterAlertFinished& InOnAlertFinished) = 0;
};
