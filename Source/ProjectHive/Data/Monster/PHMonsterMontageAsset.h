// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PHMonsterMontageAsset.generated.h"

/**
 * Author		: 임동현
 * Date			: 2025-06-15
 * Description	:
 * 몬스터가 기본으로 가지고 있어야 하는 몽타주를 가지고 있는 데이터 에셋
 */

class UAnimMontage;

UCLASS()
class PROJECTHIVE_API UPHMonsterMontageAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// Idle 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = IdleMontage)
	TObjectPtr<UAnimMontage> IdleMontage;

	// BeginAlert 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BeginAlertMontage)
	TObjectPtr<UAnimMontage> BeginAlertMontage;

	// EndAlert 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EndAlertMontage)
	TObjectPtr<UAnimMontage> EndAlertMontage;

	// Attack 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackMontage)
	TObjectPtr<UAnimMontage> AttackMontage;

	// Hit 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HitMontage)
	TObjectPtr<UAnimMontage> HitMontage;

	// Die 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DieMontage)
	TObjectPtr<UAnimMontage> DieMontage;
};
