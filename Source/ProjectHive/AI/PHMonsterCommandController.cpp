// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PHMonsterCommandController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

APHMonsterCommandController::APHMonsterCommandController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/ProjectHive/AI/BB_Command.BB_Command"));
	if (BBAssetRef.Object != nullptr)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/ProjectHive/AI/BT_CommanderMonster.BT_CommanderMonster"));
	if (BTAssetRef.Object != nullptr)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void APHMonsterCommandController::SetIsOnAlert(bool InIsOnAlert)
{
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsOnAlert"), InIsOnAlert);
	UE_LOG(LogTemp, Warning, TEXT("SetIsOnAlert"));
}
