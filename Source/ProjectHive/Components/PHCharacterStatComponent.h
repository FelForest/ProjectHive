// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PHCharacterStatComponent.generated.h"


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

protected:

	// 체력
	UPROPERTY()
	float Hp;

	// 공격력
	UPROPERTY()
	float AttackDamage;

	// 공격 속도
	UPROPERTY()
	float AttackSpeed;

	// 이동 속도
	UPROPERTY()
	float MovementSpeed;
};
