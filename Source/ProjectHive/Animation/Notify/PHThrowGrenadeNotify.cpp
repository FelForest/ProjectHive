// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/PHThrowGrenadeNotify.h"
#include "Interface/PHGrenadeThrowInterface.h"

void UPHThrowGrenadeNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
	{
		return;
	}

	IPHGrenadeThrowInterface* GrenadeThrow = Cast<IPHGrenadeThrowInterface>(MeshComp->GetOwner());

	if (GrenadeThrow == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("ThrowGrenade"));
	GrenadeThrow->ThrowGrenade();
}
