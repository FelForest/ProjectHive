// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/PHMonsterSensingComponent.h"
#include "Components/PHMonsterStatComponent.h"
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

protected:
	// 필요한 에셋 로드 함수

public:	

protected:
	// 현재는 타겟일 하나여서 이런데 추후 타겟이 여러개로 만드는 방식으로 하든지 아니면 위험도에 따라 타겟을 정하게 하는 방식으로 할듯
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

	// 죽었는지 아닌지 확인용 변수
	UPROPERTY()
	uint8 bisDead : 1;

};
