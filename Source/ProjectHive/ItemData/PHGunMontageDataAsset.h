// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PHGunMontageDataAsset.generated.h"

/**
 * Author		: 임동현
 * Date			: 2025-06-07
 * Description	:
 * 몽타주 변경 시 에디터에서 쉽게 수정할 수 있도록, 총 객체와 총 몽타주를 분리하여 데이터 에셋으로 관리
 * 기본 공격
 * 특수 공격
 * 재장전
 */

class UAnimMontage;

UCLASS()
class PROJECTHIVE_API UPHGunMontageDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// 총이 가지고 있는 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GunMontage)
	TObjectPtr<class UAnimMontage> NormalAimAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GunMontage)
	TObjectPtr<class UAnimMontage> NormalAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GunMontage)
	TObjectPtr<class UAnimMontage> SpecialAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GunMontage)
	TObjectPtr<class UAnimMontage> ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GunMontage)
	TObjectPtr<class UAnimMontage> ThrowMontage;
};
