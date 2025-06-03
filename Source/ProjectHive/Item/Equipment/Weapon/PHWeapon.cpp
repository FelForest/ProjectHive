// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equipment/Weapon/PHWeapon.h"

APHWeapon::APHWeapon()
{
	EquipmentType = EEquipmentType::Weapon;
	bIsEquipped = false;
}

void APHWeapon::Attack()
{
	UE_LOG(LogTemp, Log, TEXT("Attack!!"));
}


