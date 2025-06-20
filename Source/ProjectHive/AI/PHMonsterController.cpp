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

bool APHMonsterController::GetIsCombat() const
{
	return bIsInCombat;
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
