// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PHMonsterAttackComponent.generated.h"

/**
 * Author		: 임동현
 * Date			: 2025-06-17
 * Description	:
 * 몬스터 공격의 추상 컴포넌트
 * 실제 구현은 이것을 상속 받아서 만들어야함
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTHIVE_API UPHMonsterAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPHMonsterAttackComponent();

	// 파생 클래스에서 구현 필요
	UFUNCTION()
	virtual void Attack();

	UFUNCTION()
	virtual void SetTarget(class AActor* NewTarget) {};

	UFUNCTION()
	virtual bool CanAttack() const;

	UFUNCTION()
	void SetMonsterStat(const class UPHMonsterStatComponent* NewStat);

private:
	UFUNCTION()
	void ResetCanAttack();

protected:
	UPROPERTY()
	TObjectPtr<class AActor> Target;

	UPROPERTY()
	uint8 bCanAttack : 1;

	UPROPERTY()
	FTimerHandle AttackCooldownHandle;

	// TODO : 데이터 테이블로 받아와야함
	// 스킬 재사용 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cooldown)
	float AttackCooldown;

	UPROPERTY()
	TObjectPtr<const class UPHMonsterStatComponent> MonsterStat;
};
