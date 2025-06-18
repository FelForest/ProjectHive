// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PHAttackTaskNode.h"
#include "AIController.h"
#include "Interface/PHMonsterAttackInterface.h"

UPHAttackTaskNode::UPHAttackTaskNode()
{
}

EBTNodeResult::Type UPHAttackTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IPHMonsterAttackInterface* CombatMonster = Cast<IPHMonsterAttackInterface>(ControllingPawn);
	if (CombatMonster == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FCombatMonsterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	CombatMonster->SetCombatMonsterAttackdelegate(OnAttackFinished);

	CombatMonster->Attack();

	return EBTNodeResult::InProgress;
}
