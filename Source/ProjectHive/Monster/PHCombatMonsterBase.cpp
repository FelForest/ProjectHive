// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/PHCombatMonsterBase.h"
#include "Components/PHMonsterAttackComponent.h"
#include "Factory/PHMonsterAttackComponentFactory.h"
#include "Data/Monster/PHMonsterMontageAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

APHCombatMonsterBase::APHCombatMonsterBase()
{

}

void APHCombatMonsterBase::Attack()
{
	/*UPHMonsterAttackComponent* AttackComponent = SelectAttackComponent();
	if (AttackComponent != nullptr)
	{
		AttackComponent->Attack();
	}*/

	if (AnimInstance->Montage_IsPlaying(MonsterMontages->AttackMontage))
	{
		return;
	}

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	if (AnimInstance != nullptr && MonsterMontages && MonsterMontages->AttackMontage)
	{
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &APHCombatMonsterBase::NotifyAttackFinished);
		AnimInstance->Montage_Play(MonsterMontages->AttackMontage);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, MonsterMontages->AttackMontage);
	}
}

void APHCombatMonsterBase::SetCombatMonsterAttackdelegate(const FCombatMonsterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void APHCombatMonsterBase::NotifyAttackFinished(UAnimMontage* Montage, bool bInterrupted)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnAttackFinished.ExecuteIfBound();
}

void APHCombatMonsterBase::DoAttack()
{
	// 트레이스든 콜리전이든 생성 필요
}

UPHMonsterAttackComponent* APHCombatMonsterBase::SelectAttackComponent()
{
	return nullptr;
}

//void APHCombatMonsterBase::SetAttackPattern(const UPHMonsterAttackPatternData* AttackData)
//{
//	if (AttackData == nullptr)
//	{
//		UE_LOG(LogTemp, Log, TEXT("AttackData is nullptr"));
//		return;
//	}
//
//	for (const auto& NewAttackData : AttackData->MonsterAttackPattern)
//	{
//		AttackPattern.Add({ UPHMonsterAttackComponentFactory::CreateMonsterAttackComponent(this, NewAttackData.AttackType), NewAttackData.Montage });
//	}
//}
