// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/PHMonsterSensingComponent.h"
#include "Components/PHMonsterStatComponent.h"

#include "Interface/PHAlertCallerInterface.h"

#include "PHMonsterBase.generated.h"

/**
 * Author		: 임동현
 * Date			: 2025-06-11
 * Description	:
 * 몬스터의 기본이 되는 클래스
 * 감지
 * 감지 시 주변에 전파
 * 이동
 * 흥분도 시스템
 * 흥분 레벨이 높아질 수록 스탯에 부과 효과 증가
 * 죽음
 * 
 * 소유 컴포넌트
 * 감지 컴포넌트
 * 감지시 주위에게 감지한것을 전파시키는 것
 * 만약 피격시 감지함, 감지했는데 없으면 그쪽 위치로 가자고 명령을 내려야함
 * 이건 캐릭터에서 컴포넌트한테 target을 던져줘야 할듯?
 * 감지했는데 없으면 target 위치로 moveto 하면 되고 -> 주위애들한테 moveto 하라고 알려줘야 할듯
 * 있으면 그냥 전파 시키면 되고 -> 이건 몬스터가 해야하기 때문에 인터페이스로 캐스팅해서 실행시키게 해야할듯
 * 스탯 컴포넌트
 *
 * 고민 사항
 * Attackable Interface를 만들어야 하나
 */

UCLASS()
class PROJECTHIVE_API APHMonsterBase : public ACharacter,
	public IPHAlertCallerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APHMonsterBase();

	// 목표(플레이어 및 기타 등등)을 발견하면 주위에 알리는 함수
	virtual void CallAlertTarget() override;

	// 특정 위치에 가라고 알리는 함수
	virtual void CallAlertDestination() override;

	// 몽타주 호출을 위한 함수
	virtual void CallAlertBegin(AActor* NewTarget) override;
	virtual void CallAlertBegin(FVector NewLocation) override;

	// Target 설정함수
	UFUNCTION()
	FORCEINLINE void SetTarget(AActor* NewTarget)
	{
		Target = NewTarget;
	}

	UFUNCTION()
	FORCEINLINE AActor* GetTarget() const
	{
		return Target;
	}

	UFUNCTION()
	FORCEINLINE void SetDestination(FVector NewDestination)
	{
		Destination = NewDestination;
	}

	UFUNCTION()
	FORCEINLINE FVector GetDestination() const
	{
		return Destination;
	}

	// 피격 받았을때 처리할 함수
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// 필요한 에셋 로드 함수
public:	

protected:
	// 현재는 타겟일 하나여서 이런데 추후 타겟이 여러개로 만드는 방식으로 하든지 아니면 위험도에 따라 타겟을 정하게 하는 방식으로 할듯
	// 추후 여러명 이 나오면 일단 가까운 놈을 노리는 방식으로 하던가, 위험도에 따른것으로 하던가
	// 일단은 자신을 때린 놈을 타겟으로 잡음
	// 한번 정해지면 변경 못하게 막을듯
	TObjectPtr<AActor> Target;

	// 감지를 위한 컴포넌트
	TObjectPtr<class UPHMonsterSensingComponent> SensingComponent;

	// 스탯 컴포넌트
	TObjectPtr<class UPHMonsterStatComponent> StatComponent;

	// 광폭화 비율? 흥분도
	// 몬스터가 흥분도에 따라 능력치가 변하게 변경
	// TODO : 현재는 전투 시간에 따라 변경하는 것정도로 간단하지만 조건이 많아지면 컴포넌트로 분리해야함
	UPROPERTY()
	float RageLevel;

	// 이동 할 위치 -> 처음에는 Location 했는데 TargetLocation이라고 하니까 위의 Target의 위치인지 목표 위치인지 햇갈려셔 변경
	UPROPERTY()
	FVector Destination;

	// 죽었는지 아닌지 확인용 변수
	UPROPERTY()
	uint8 bisDead : 1;
};
