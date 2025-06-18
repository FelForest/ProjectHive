// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "PHInRangeDecorator.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API UPHInRangeDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
	UPHInRangeDecorator();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
