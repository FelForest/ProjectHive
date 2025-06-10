// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PHGrenadeComponent.generated.h"

/**
 * Author		: 임동현
 * Date			: 2025-06-09
 * Description	:
 * 수류탄 관리하는 컴포넌트
 * 던지기 호출시 수류탄 생성
 * 방향 설정
 * 수류탄 갯수 관리하기
 */


class APHGrenade;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTHIVE_API UPHGrenadeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPHGrenadeComponent();

	// 수류탄 던지는 함수
	void ThrowGrenade(class USkeletalMeshComponent* InMesh, FVector InTargetLocation);

protected:
	// 수류탄 변수 설정 하는 함수
	void SetGrenadeData(USkeletalMeshComponent* InMesh, FVector InTargetLocation);

	// 수류탄 생성 함수 
	void SpawnGrenade();

	// 가져온 데이터 적용하는 함수
	void ApplyGrenadeData();

	// 수류탄 종류 고르는 함수 -> 이건 추가 예정 -> 수류탄 종류 많아지면 함

public:
	
protected:
	// 현재 수류탄 타입
	APHGrenade* CurrentGrenade;

	// 수류탄 갯수
	int32 GrenadeCount;

	// 수류탄 초기화에 필요한 변수 -> 이건 데이터 테이블로 들고올거임

	// 최초 속도
	float InitSpeed;

	// 최고 속도
	float MaxSpeed;

	// 회전 여부...-> 잘 모르겠음

	// 생성 위치값
	UPROPERTY()
	FVector SpawnLocation;

	// 생성 회전값
	UPROPERTY()
	FRotator SpawnRotation;

	// 목표 위치값
	UPROPERTY()
	FVector TargetLocation;
};
