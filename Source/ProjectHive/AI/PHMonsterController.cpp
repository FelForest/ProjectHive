// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PHMonsterController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

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

void APHMonsterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
