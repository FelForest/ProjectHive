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
class PROJECTHIVE_API APHMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APHMonsterBase();

	// 목표(플레이어 및 기타 등등)을 발견하면 주위에 알리는 함수
	virtual void CallAlertTarget();

	// 몽타주 호출을 위한 함수
	UFUNCTION()
	virtual void CallAlertTargetBegin(APawn* NewTarget);

	UFUNCTION()
	virtual void CallAlertTargetEnd();

	// 컨트롤러가 있다는것을 보장이 가능한 함수
	virtual void PossessedBy(AController* NewController) override;

	// Target 설정함수
	UFUNCTION()
	virtual void SetTarget(APawn* NewTarget);

	UFUNCTION()
	APawn* GetTarget() const;

	UFUNCTION()
	void SetIsCombat(bool NewIsCombat);

	UFUNCTION()
	bool GetIsCombat() const;

	// 피격 받았을때 처리할 함수
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// 체력이 없어 죽었을때 호출한 함수
	UFUNCTION()
	virtual void OnDead();

	UFUNCTION()
	bool GetIsDead() const;

	UFUNCTION()
	void FinializeDeath(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable, Category = "Commander")
	void SetCommander(class APHMonsterBase* NewCommander);

	UFUNCTION()
	APHMonsterBase* GetCommander();

protected:
	// 필요한 에셋 로드 함수

	// 컴포넌트의 델리게이트 바인딩 하려고 필요
	virtual void PostInitializeComponents() override;
public:	

	// 기본적으로 필요한 몬스터 몽타주 데이터 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	TObjectPtr<class UPHMonsterMontageAsset> MonsterMontages;

	FMonsterAlertFinished OnAlertFinished;

protected:
	// 감지를 위한 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sens)
	TObjectPtr<class UPHMonsterSensingComponent> SensingComponent;

	// 스탯 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	TObjectPtr<class UPHMonsterStatComponent> StatComponent;

	// 광폭화 비율? 흥분도
	// 몬스터가 흥분도에 따라 능력치가 변하게 변경
	// TODO : 현재는 전투 시간에 따라 변경하는 것정도로 간단하지만 조건이 많아지면 컴포넌트로 분리해야함
	UPROPERTY()
	float RageLevel;

	// 죽었는지 아닌지 확인용 변수
	UPROPERTY()
	uint8 bIsDead : 1;

	UPROPERTY()
	UAnimInstance* AnimInstance;

	// 얼마나 포효할지 정하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Alert)
	float AlertTime;

	UPROPERTY()
	uint8 bIsAlerting : 1;

	UPROPERTY()
	TObjectPtr<class APHMonsterBase> Commander;

private:
	UPROPERTY()
	TScriptInterface<class IPHSensingAIInterface> SensingAI;

	UPROPERTY()
	TScriptInterface<class IPHMonsterAIInterface> MonsterAI;

	FTimerHandle AlertTimer;
};
