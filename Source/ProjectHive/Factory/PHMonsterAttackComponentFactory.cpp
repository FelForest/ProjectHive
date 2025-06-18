// Fill out your copyright notice in the Description page of Project Settings.


#include "Factory/PHMonsterAttackComponentFactory.h"
#include "Components/PHMonsterAttackComponent.h"
#include "Components/MonstAttack/PHBiteAttackComponent.h"

UPHMonsterAttackComponent* UPHMonsterAttackComponentFactory::CreateMonsterAttackComponent(AActor* Owner, EMonsterAttack InAttackType)
{
	if (Owner == nullptr)
	{
		return nullptr;
	}

	UPHMonsterAttackComponent* NewMonsterAttackComponent = nullptr;

	switch (InAttackType)
	{
	case EMonsterAttack::Bite:
		NewMonsterAttackComponent = NewObject<UPHBiteAttackComponent>(Owner, TEXT("BiteAttackComponent"));
		break;
	case EMonsterAttack::Shoot:
		//NewMonsterAttackComponent = NewObject<UPHRangedAttackComponent>(Owner, TEXT("ShootAttackComponent"));
		break;
	default:
		break;
	}
	return NewMonsterAttackComponent;
}
