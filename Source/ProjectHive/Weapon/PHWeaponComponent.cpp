// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PHWeaponComponent.h"
#include "Item/Equipment/Weapon/PHWeapon.h"

// Sets default values for this component's properties
UPHWeaponComponent::UPHWeaponComponent()
{
}

void UPHWeaponComponent::Attack()
{
	if (Weapon != nullptr)
	{
		Weapon->Attack();
	}
}

void UPHWeaponComponent::SetWeapon(APHEquipment* InEquipment)
{
	// 기존의 장비가 변경되는 경우에도 이 함수 호출
	// 이때 먼저 장비가 있는지 확인 후 기존의 바인딩된 함수 다 제거해야함
	if (Weapon != nullptr)
	{
		// TODO : 여기서 제거 필요
	}

	// Use Delegate to UI
	Weapon = Cast<APHWeapon>(InEquipment);

	// 무기가 아니라고 판단
	if (Weapon == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon is nullptr"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Weapon Equipped : %s"), *Weapon->GetName());

	// TODO : 발사 결과 값에 따라 애니메이션 다르게 하려고함
	// 바인딩 위치는 여기가 맞다고 판단
	// 캐릭터를 인터페이스로 캐스팅 후 바인딩할 예정
}

void UPHWeaponComponent::ClearWeapon(APHEquipment* InEquipment)
{
	if (Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon is Clear"));
		// TODO : 기존 무기가 바인딩된 함수 제거 필요
		Weapon = nullptr;
	}
}

APHEquipment* UPHWeaponComponent::GetWeapon() const
{
	return Cast<APHEquipment>(Weapon);
}

void UPHWeaponComponent::InitializeWeaponMesh(USkeletalMeshComponent* CharacterMesh)
{
}

