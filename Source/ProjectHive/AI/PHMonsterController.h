// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Interface/PHSensingAIInterface.h"
#include "AI/PHMonsterAIInterface.h"
#include "PHMonsterController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API APHMonsterController : public AAIController,
	public IPHSensingAIInterface,
	public IPHMonsterAIInterface

{
	GENERATED_BODY()
	
public:
	APHMonsterController();

	virtual void RunAI() override;
	virtual void StopAI() override;

	// 전투중인지 아닌지는 몬스터가 판단해도 될거 같음
	virtual void SetIsCombat(bool NewState) override;

	virtual void SetInRange(bool NewInRange) override;

	virtual float GetRange() const override;

	virtual void SetCanAlert(bool NewCanAlert) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	// Interface Section
	// 몬스터의 센싱 컴포넌트의 델리게이트에 바이딩할 함수
	virtual void SetTarget(APawn* NewPawn) override;

	virtual APawn* GetTarget() const override;

	virtual void SetDestination(FVector NewDestination) override;

	virtual FVector GetDestination() const override;

protected:
	// Blackboard 애셋
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	// Behavior Tree 애셋
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	// 목표 : 현재는 플레이어 추후 다른게 추가 될 수도 있음
	// 현재는 타겟일 하나여서 이런데 추후 타겟이 여러개로 만드는 방식으로 하든지 아니면 위험도에 따라 타겟을 정하게 하는 방식으로 할듯
	// 추후 여러명 이 나오면 일단 가까운 놈을 노리는 방식으로 하던가, 위험도에 따른것으로 하던가
	// 일단은 자신을 때린 놈을 타겟으로 잡음
	// 한번 정해지면 변경 못하게 막을듯
	UPROPERTY()
	TObjectPtr<APawn> Target;

	UPROPERTY()
	FVector Destination;

	UPROPERTY()
	uint8 bIsInCombat : 1;
};
