// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PHWeaponComponent.h"
#include "Item/Equipment/Weapon/PHWeapon.h"

#include "ItemData/PHGunMontageDataAsset.h"
#include "Item/Equipment/Weapon/Gun/PHGun.h"

// Sets default values for this component's properties
UPHWeaponComponent::UPHWeaponComponent()
{
	WeaponSettingMap.Add(EWeaponType::Gun, &UPHWeaponComponent::SetGun);
}

void UPHWeaponComponent::Attack()
{
	if (CanAttack())
	{
		CurrentWeapon->Attack();
	}
}

void UPHWeaponComponent::SetWeapon(APHEquipment* InWeapon)
{
	// 기존의 장비가 변경되는 경우에도 이 함수 호출
	// 이때 먼저 장비가 있는지 확인 후 기존의 바인딩된 함수 다 제거해야함

	// Use Delegate to UI

	if (InWeapon == nullptr)
	{
		CurrentWeapon = nullptr;
		return;
	}

	if (InWeapon->GetEquipmentType() != EEquipmentType::Weapon)
	{
		return;
	}

	APHWeapon* NewWeapon = Cast<APHWeapon>(InWeapon);
	if (NewWeapon == nullptr)
	{
		return;
	}

	// 무기 설정
	CurrentWeapon = NewWeapon;
	EWeaponType WeaponType = CurrentWeapon->GetWeaponType();

	// 여기서 어떤 무기인지 분기로 나누어야함
	if (WeaponSetupFunc* WeaponSetupFunc = WeaponSettingMap.Find(WeaponType))
	{
		(this->** WeaponSetupFunc)();
	}

	// TODO : 발사 결과 값에 따라 애니메이션 다르게 하려고함
	// 바인딩 위치는 여기가 맞다고 판단
	// 캐릭터를 인터페이스로 캐스팅 후 바인딩할 예정


}

void UPHWeaponComponent::ClearWeapon(APHEquipment* InWeapon)
{
	if (CurrentWeapon == InWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon is Clear"));
		// TODO : 기존 무기가 바인딩된 함수 제거 필요

		CurrentWeapon->SetIsEquipped(false);

		CurrentWeapon = nullptr;
	}
}

APHWeapon* UPHWeaponComponent::GetWeapon() const
{
	return CurrentWeapon;
}

bool UPHWeaponComponent::CanAttack()
{
	if (CurrentWeapon != nullptr && CurrentWeapon->CanAttack())
	{
		return true;
	}
	return false;
}

UAnimMontage* UPHWeaponComponent::GetNormalAimAttackMontage()
{
	return NormalAimAttackMontage;
}

UAnimMontage* UPHWeaponComponent::GetNormalAttackMontage()
{
	return NormalAttackMontage;
}

UAnimMontage* UPHWeaponComponent::GetSpecialAttackMontage()
{
	return SpecialAttackMontage;
}

UAnimMontage* UPHWeaponComponent::GetReloadMontage()
{
	return ReloadMontage;
}

UAnimMontage* UPHWeaponComponent::GetThrowMontage()
{
	return ThrowMontage;
}

bool UPHWeaponComponent::CanReload()
{
	IPHReloadInterface* Reloadable = Cast<IPHReloadInterface>(CurrentWeapon);
	if (Reloadable != nullptr)
	{
		return Reloadable->CanReload();
	}
	return false;
}

void UPHWeaponComponent::ReloadStart()
{
	IPHReloadInterface* Reloadable = Cast<IPHReloadInterface>(CurrentWeapon);
	if (Reloadable != nullptr && Reloadable->CanReload())
	{
		Reloadable->ReloadStart();
	}
}

void UPHWeaponComponent::ReloadEnd(UAnimMontage* Montage, bool bInterrupted)
{
	IPHReloadInterface* Reloadable = Cast<IPHReloadInterface>(CurrentWeapon);
	if (Reloadable != nullptr && Reloadable->IsReloading())
	{
		Reloadable->ReloadEnd();
	}
}

void UPHWeaponComponent::InitializeWeaponMesh(USkeletalMeshComponent* CharacterMesh)
{

}

void UPHWeaponComponent::SetGun()
{
	// 총으로 캐스팅 하기
	APHGun* NewGun = Cast<APHGun>(CurrentWeapon);
	if (NewGun == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("This Weapon is not gun. Check WeaponType is Gun"));
		return;
	}

	// 몽타주 데이터 에셋 가져오기
	UPHGunMontageDataAsset* MontageData = NewGun->GetGunMontageData();
	if (MontageData == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Gun Montage data is nullptr"));
		return;
	}

	// 새 몽타주 가져오기
	NormalAimAttackMontage = MontageData->NormalAimAttackMontage;
	if (NormalAimAttackMontage == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("NormalAimAttackMontage is nullptr"));
	}

	NormalAttackMontage = MontageData->NormalAttackMontage;
	if (NormalAttackMontage == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("NormalAttackMontage is nullptr"));
	}

	SpecialAttackMontage = MontageData->SpecialAttackMontage;
	if (SpecialAttackMontage == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("SpecialAttackMontage is nullptr"));
	}

	ReloadMontage = MontageData->ReloadMontage;
	if (ReloadMontage == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ReloadMontage is nullptr"));
	}

	ThrowMontage = MontageData->ThrowMontage;
	if (ThrowMontage == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ThrowMontage is nullptr"));
	}

	// TODO : 델리게이트로 알맞은 UI 호출하게 해야함
	// 넘겨줄 데이터, 무기 이미지
	OnGunEquipped.Broadcast(NewGun);
}


