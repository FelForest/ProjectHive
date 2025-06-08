// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/PHReloadNotify.h"
#include "Character/PHPlayableCharacter.h"
#include "Weapon/PHWeaponComponent.h"
#include "Item/Equipment/Weapon/PHWeapon.h"
#include "Interface/PHReloadInterface.h"

void UPHReloadNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("NotifyMesh is nullptr"));
		return;
	}

	// SetLeaderPoseComponent로 설정 되어 있어서 캐릭터에서만 호출이 됨
	// 즉 캐릭터로 캐스팅 해야한다는 뜻
	APHPlayableCharacter* Character = Cast<APHPlayableCharacter>(MeshComp->GetOwner());

	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Notify Character is nullptr"));
		return;
	}

	IPHReloadInterface* Reloadable = Cast<IPHReloadInterface>(Character->GetWeaponComponent()->GetWeapon());

	if (Reloadable == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Notify Reloadable is nullptr"));
		return;
	}

	Reloadable->Reload();
}
