// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PHMonsterStatComponent.generated.h"

/**
 * Author		: 임동현
 * Date			: 2025-06-20
 * Description	:
 * 몬스터가 최소한으로 가지고 있어야 하는 값들
 * 필요시 상속 받아어 사용 바람
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHPChange, float /*HP*/);
DECLARE_MULTICAST_DELEGATE(FOnDead);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTHIVE_API UPHMonsterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPHMonsterStatComponent();

	UFUNCTION()
	FORCEINLINE float GetMoveSpeed() const
	{
		return MoveSpeed;
	}

	UFUNCTION()
	FORCEINLINE float GetHP() const
	{
		return Hp;
	}

	UFUNCTION()
	FORCEINLINE float GetAlertRadius() const
	{
		return AlertRadius;
	}

protected:
	
	
public:	
	FOnHPChange OnHPChange;

	FOnDead OnDead;

public:
	UFUNCTION()
	void ChangeHP(float InDamage);



protected:
	// 전파 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AlertRadius;

	// 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Hp;

	// 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MoveSpeed;
};
