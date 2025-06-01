// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equipment/Weapon/PHWeapon.h"

APHWeapon::APHWeapon()
{
	EquipmentType = EEquipmentType::Weapon;
}

void APHWeapon::Attack()
{
	// TODO : 여기서 히트스캔 하는거 만들어야함
	UE_LOG(LogTemp, Log, TEXT("Attack!!"));
}
