// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PHInRangeDecorator.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/PHMonsterAIInterface.h"
#include "Monster/PHMonsterBase.h"

UPHInRangeDecorator::UPHInRangeDecorator()
{
	NodeName = TEXT("InRange");
}

bool UPHInRangeDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControlleringPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControlleringPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ControlleringPawn!!!!!!"));
		return false;
	}

	APHMonsterBase* Monster = Cast<APHMonsterBase>(ControlleringPawn);
	if (Monster == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Monster!!!!!!"));
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (Target == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target!!!!!!"));
		return false;
	}

	float DistanceToTarget = ControlleringPawn->GetDistanceTo(Target);
	const float AttackRange = Monster->GetAttackRange();
	bResult = (DistanceToTarget <= AttackRange);

	return bResult;
}


