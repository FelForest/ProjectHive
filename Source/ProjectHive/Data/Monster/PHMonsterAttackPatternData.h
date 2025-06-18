// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/Monster/PHMonsterAttack.h"
#include "PHMonsterAttackPatternData.generated.h"

/**
 * Author		: 임동현
 * Date			: 2025-06-17
 * Description	:
 * 몬스터 공격 순서 정할 수 있는 데이터 에셋
 * 
 */


USTRUCT()
struct FMonsterAttack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = AttackType)
	EMonsterAttack AttackType = EMonsterAttack::None;

	UPROPERTY(EditAnywhere, Category = Montage)
	TObjectPtr<class UAnimMontage> Montage;

};

UCLASS()
class PROJECTHIVE_API UPHMonsterAttackPatternData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<FMonsterAttack> MonsterAttackPattern;
};
