// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PHAlertTaskNode.h"
#include "AIController.h"
#include "Interface/PHAlertCallerInterface.h"
#include "BehaviorTree/BlackboardComponent.h"


UPHAlertTaskNode::UPHAlertTaskNode()
{
	NodeName = TEXT("Alert Destination");
}

EBTNodeResult::Type UPHAlertTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IPHAlertCallerInterface* AlertMonster = Cast<IPHAlertCallerInterface>(ControllingPawn);
	if (AlertMonster == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	if (AlertMonster->IsAlerting())
	{
		return EBTNodeResult::Succeeded;
	}

	FMonsterAlertFinished OnAlertFinished;
	OnAlertFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	AlertMonster->SetMonsterAlertDelegate(OnAlertFinished);

	AlertMonster->CallAlertDestinationBegin(Target);

	return EBTNodeResult::InProgress;
}
