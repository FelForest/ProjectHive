// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PHCharacterStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnHPChange, float /*HP*/);
DECLARE_MULTICAST_DELEGATE(FOnDead);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTHIVE_API UPHCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPHCharacterStatComponent();

	// 데이터 에셋으로 받아와서 스탯 초기화
	UFUNCTION()
	void InitializeStat();

	UFUNCTION()
	void ChangeHP(float InDamage);

public:
	FOnHPChange OnHPChange;

	FOnDead OnDead;

public:

	// 체력
	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float Hp;

	UPROPERTY()
	float HpPersent;

	// 공격력
	UPROPERTY()
	float AttackDamage;

	// 공격 속도
	UPROPERTY()
	float AttackSpeed;

	// 이동 속도
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = MoveSpeed, meta = (AllowPrivateAccess = "true"))
	float WalkMovementSpeed;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = MoveSpeed, meta = (AllowPrivateAccess = "true"))
	float RunMovementSpeed;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = MoveSpeed, meta = (AllowPrivateAccess = "true"))
	float AimMovementSpeed;
};
