// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PHMonsterController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/PHMonsterBase.h"

APHMonsterController::APHMonsterController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/ProjectHive/AI/BB_Monster.BB_Monster"));
	if (BBAssetRef.Object != nullptr)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/ProjectHive/AI/BT_Monster.BT_Monster"));
	if (BTAssetRef.Object != nullptr)
	{
		BTAsset = BTAssetRef.Object;
	}

	bIsInCombat = false;
}

void APHMonsterController::RunAI()
{
	UBlackboardComponent* BlackBoardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackBoardPtr))
	{
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void APHMonsterController::StopAI()
{
	UBehaviorTreeComponent* BTComponenet = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponenet != nullptr)
	{
		BTComponenet->StopTree();
	}
}

void APHMonsterController::SetIsCombat(bool NewState)
{
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsInCombat"), NewState);
	bIsInCombat = NewState;
}

void APHMonsterController::SetInRange(bool NewInRange)
{
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsInRange"), NewInRange);
}

float APHMonsterController::GetRange() const
{
	APHMonsterBase* Monster = Cast<APHMonsterBase>(GetPawn());
	if (Monster)
	{
		return Monster->GetAttackRange();
	}
	return 0.0f;
}

void APHMonsterController::SetCanAlert(bool NewCanAlert)
{
	GetBlackboardComponent()->SetValueAsBool(TEXT("CanAlert"), NewCanAlert);
}

void APHMonsterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunAI();
}

void APHMonsterController::SetTarget(APawn* NewPawn)
{
	GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), NewPawn);
	Target = NewPawn;
}

APawn* APHMonsterController::GetTarget() const
{
	return Target;
}

void APHMonsterController::SetDestination(FVector NewDestination)
{
	GetBlackboardComponent()->SetValueAsVector(FName("Destination"), NewDestination);
	Destination = NewDestination;
}

FVector APHMonsterController::GetDestination() const
{
	return Destination;
}
