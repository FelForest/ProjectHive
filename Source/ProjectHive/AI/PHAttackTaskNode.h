// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PHAttackTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API UPHAttackTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPHAttackTaskNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
