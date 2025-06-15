// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PHSensingAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPHSensingAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Author		: 임동현
 * Date			: 2025-06-14
 * Description	:
 * 몬스터의 타겟을 설정하기 위한 인터페이스
 */
class PROJECTHIVE_API IPHSensingAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void SetTarget(class APawn* NewTarget) = 0;

	UFUNCTION()
	virtual APawn* GetTarget() const = 0;

	UFUNCTION()
	virtual void SetDestination(FVector NewDestination) = 0;

	UFUNCTION()
	virtual FVector GetDestination() const = 0;
};
