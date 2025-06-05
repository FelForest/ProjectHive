// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PHWeaponComponent.h"
#include "Item/Equipment/Weapon/PHWeapon.h"

// Sets default values for this component's properties
UPHWeaponComponent::UPHWeaponComponent()
{
}

void UPHWeaponComponent::Attack()
{
	if (CurrentWeapon != nullptr)
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

	// 여기서 어떤 무기인지 분기로 나누어야함
	
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

void UPHWeaponComponent::InitializeWeaponMesh(USkeletalMeshComponent* CharacterMesh)
{
}


