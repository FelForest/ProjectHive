// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PHClearAlertTaskNode.h"
#include "AIController.h"
#include "Interface/PHCommandAIInterface.h"


EBTNodeResult::Type UPHClearAlertTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	IPHCommandAIInterface* CommandController = Cast<IPHCommandAIInterface>(OwnerComp.GetAIOwner());
	if (CommandController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	CommandController->SetIsOnAlert(false);

	return EBTNodeResult::Succeeded;
}
