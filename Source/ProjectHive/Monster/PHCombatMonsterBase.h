// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/PHMonsterBase.h"
#include "Interface/PHMonsterAttackInterface.h"
#include "Data/Monster/PHMonsterAttackPatternData.h"

#include "PHCombatMonsterBase.generated.h"

/**
 * Author		: 임동현
 * Date			: 2025-06-17
 * Description	:
 * 전투 가능한 몬스터의 기본이 되는 클래스(이것도 추상 클래스)
 * 공격 인터페이스를 구현
 * 공격 컴포넌트를 배열도 들고 있어서 어떠한 공격이 가능한지 판단 가능
 * 어떠한 공격을 할지 선택하는 기능
 */



class UPHMonsterAttackComponent;
class UAnimMontage;

// 순서 보장하면서 중복 제거 하면서 구조체 가지고 있게 하고 싶은데 방법이 없나?
// 에디터에서 수정 때리고 싶은데
//USTRUCT()
//struct FMonsterPattern
//{
//	GENERATED_BODY()
//
//	UPROPERTY()
//	TObjectPtr<UPHMonsterAttackComponent> MonsterAttack;
//
//	UPROPERTY()
//	TObjectPtr<UAnimMontage> AttackMontage;
//};

UCLASS()
class PROJECTHIVE_API APHCombatMonsterBase : public APHMonsterBase,
	public IPHMonsterAttackInterface
{
	GENERATED_BODY()
	
public:
	APHCombatMonsterBase();

	virtual void Attack() override;

	virtual void SetCombatMonsterAttackdelegate(const FCombatMonsterAttackFinished& InOnAttackFinished) override;

	virtual void NotifyAttackFinished(UAnimMontage* Montage, bool bInterrupted) override;

	virtual void DoAttack() override;

protected:
	// 파생 클래스에서 재정의 필요
	virtual UPHMonsterAttackComponent* SelectAttackComponent();

	//// 공격패턴을 위한 배열 설정을 해야할듯?
	//virtual void SetAttackPattern(const UPHMonsterAttackPatternData* DataAsset);

protected:
	// 공격 컴포넌트를 담아놓은 Map
	// CONSIDER : 다른 자료구조 생각해 보기
	/*UPROPERTY()
	TArray<FMonsterPattern> AttackPattern;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackPattern)
	TObjectPtr<UPHMonsterAttackPatternData> PHMonsterAttackPatternData;*/

	FCombatMonsterAttackFinished OnAttackFinished;
};
