// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PHGunAnimInstance.generated.h"

/**
 * Author		: 임동현
 * Date			: 2025-06-04
 * Description	:
 * 이동
 * 조준
 * 
 * 몽타주
 * 공격
 * 무기 변경
 * 수류탄 던지기
 * 상호작용하는 상태
 * 죽는 모션
 *
 */
UCLASS()
class PROJECTHIVE_API UPHGunAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPHGunAnimInstance();

	// Native initialization override point
	virtual void NativeInitializeAnimation() override;

	// Native update override point. It is usually a good idea to simply gather data in this step and 
	// for the bulk of the work to be done in NativeThreadSafeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Native thread safe update override point. Executed on a worker thread just prior to graph update 
	// for linked anim instances, only called when the hosting node(s) are relevant
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

public:
	// 애니메이션을 재생하는 캐릭터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class APHPlayableCharacter> Owner;

	// 캐릭터 무브먼트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	// 무기를 관리하는 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UPHWeaponComponent> WeaponComponent;

	// 현재 장착중인 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class APHWeapon> Weapon;

	// 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	// 땅에서의 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	// 현재 움직이는지 아닌지확인용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	// 현재 액터 앞 방향
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	FRotator Rotation;

	// 조준 중 이동 방향과 바라보는 방향 사이의 각도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float AimDirection;

	// 현재 조준중인지 아닌지 확인용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gun)
	uint8 bIsAiming : 1;

	// 폰의 앞 방향
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gun)
	FVector Forward;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gun)
	uint8 bIsEquipped : 1;

	// 경계 상태를 나타내는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gun)
	float TensionLevel;
};
