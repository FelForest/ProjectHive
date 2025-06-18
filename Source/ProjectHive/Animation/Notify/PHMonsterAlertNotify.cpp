// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/PHMonsterAlertNotify.h"
#include "Interface/PHAlertCallerInterface.h"

void UPHMonsterAlertNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("NotifyMesh is nullptr"));
		return;
	}

	// SetLeaderPoseComponent로 설정 되어 있어서 캐릭터에서만 호출이 됨
	// 즉 캐릭터로 캐스팅 해야한다는 뜻
	IPHAlertCallerInterface* AlertCaller = Cast<IPHAlertCallerInterface>(MeshComp->GetOwner());

	if (AlertCaller == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Notify AlertCaller is nullptr"));
		return;
	}

	AlertCaller->CallAlertDestination();
}
