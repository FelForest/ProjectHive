// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equipment/Weapon/PHWeapon.h"

APHWeapon::APHWeapon()
{
	EquipmentType = EEquipmentType::Weapon;
	bIsEquipped = false;

	static ConstructorHelpers::FClassFinder<UAnimInstance> CharacterAnim(TEXT("/Game/ProjectHive/Animation/NewFolder/ABP_AR.ABP_AR_C"));

	if (CharacterAnim.Class)
	{
		WeaponAnimClass = CharacterAnim.Class;
		EquipmentMesh->SetAnimClass(CharacterAnim.Class);
	}
}

void APHWeapon::Attack()
{
	UE_LOG(LogTemp, Log, TEXT("%s : Attack!!"), *GetName());
}

bool APHWeapon::CanAttack()
{
	return false;
}

EWeaponType APHWeapon::GetWeaponType() const
{
	return WeaponType;
}


