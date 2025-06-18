// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PHAlertTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API UPHAlertTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPHAlertTaskNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
