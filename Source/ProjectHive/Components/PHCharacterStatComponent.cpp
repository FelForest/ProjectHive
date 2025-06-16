// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PHCharacterStatComponent.h"

// Sets default values for this component's properties
UPHCharacterStatComponent::UPHCharacterStatComponent()
{
	// Setting HP
	MaxHp = 100.0f;
	Hp = MaxHp;

	HpPersent = Hp / MaxHp;

	// Setting Speed
	WalkMovementSpeed = 625.0f;
	RunMovementSpeed = 1000.0f;
	AimMovementSpeed = 400.0f;
}

void UPHCharacterStatComponent::InitializeStat()
{
}

void UPHCharacterStatComponent::ChangeHP(float InDamage)
{
	// 내부에서 방어력이나 이런거 다 해줘야 하는데 지금은 없으니까 패스
	Hp -= InDamage;

	// 죽었을때
	if (Hp <= KINDA_SMALL_NUMBER)
	{
		Hp = 0.0f;
		OnDead.Broadcast();
	}

	HpPersent = Hp / MaxHp;

	OnHPChange.Broadcast(HpPersent);
}
