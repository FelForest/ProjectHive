// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/PHChangeWeaponNotify.h"
#include "Interface/PHWeaponChangeInterface.h"

void UPHChangeWeaponNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	IPHWeaponChangeInterface* WeaponChangeAble = Cast<IPHWeaponChangeInterface>(MeshComp->GetOwner());

	if (WeaponChangeAble == nullptr)
	{
		return;
	}

	WeaponChangeAble->ChangeWeapon();

}
