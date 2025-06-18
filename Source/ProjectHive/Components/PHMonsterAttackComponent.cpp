// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PHMonsterAttackComponent.h"
#include "Components/PHMonsterStatComponent.h"

UPHMonsterAttackComponent::UPHMonsterAttackComponent()
{
	bCanAttack = true;
	AttackCooldown = 2.0f;
}

void UPHMonsterAttackComponent::Attack()
{
	bCanAttack = false;

	GetWorld()->GetTimerManager().ClearTimer(AttackCooldownHandle);
	GetWorld()->GetTimerManager().SetTimer(
		AttackCooldownHandle,
		this,
		&UPHMonsterAttackComponent::ResetCanAttack,
		AttackCooldown,
		false);
}

bool UPHMonsterAttackComponent::CanAttack() const
{
	return bCanAttack;
}

void UPHMonsterAttackComponent::SetMonsterStat(const UPHMonsterStatComponent* NewStat)
{
	MonsterStat = NewStat;
}

void UPHMonsterAttackComponent::ResetCanAttack()
{
	bCanAttack = true;
}
