// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PHPatrollerDestinationTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API UPHPatrollerDestinationTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
