// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/PHMonsterAttackNotify.h"
#include "Interface/PHMonsterAttackInterface.h"

void UPHMonsterAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("NotifyMesh is nullptr"));
		return;
	}

	IPHMonsterAttackInterface* AttackInterface = Cast<IPHMonsterAttackInterface>(MeshComp->GetOwner());

	if (AttackInterface == nullptr)
	{
		//UE_LOG(LogTemp, Log, TEXT("Notify AttackInterface is nullptr"));
		return;
	}

	AttackInterface->Attack();
}
