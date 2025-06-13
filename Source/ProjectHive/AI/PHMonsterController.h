// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PHMonsterController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API APHMonsterController : public AAIController
{
	GENERATED_BODY()
	
public:
	APHMonsterController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	// Blackboard 애셋
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	// Behavior Tree 애셋
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
