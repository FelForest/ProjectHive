// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PHMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API UPHMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPHMonsterAnimInstance();

	// Native initialization override point
	virtual void NativeInitializeAnimation() override;

	// Native update override point. It is usually a good idea to simply gather data in this step and 
	// for the bulk of the work to be done in NativeThreadSafeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Native thread safe update override point. Executed on a worker thread just prior to graph update 
	// for linked anim instances, only called when the hosting node(s) are relevant
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	// 애니메이션을 재생하는 캐릭터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Monster)
	TObjectPtr<class APHCombatMonsterBase> Owner;

	// 캐릭터 무브먼트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Monster)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	// 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster)
	FVector Velocity;

	// 땅에서의 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Monster)
	float GroundSpeed;

	// 현재 움직이는지 아닌지확인용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Monster)
	uint8 bIsIdle : 1;

	// 현재 경고중인지 알리는용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Monster)
	uint8 bIsAlerting : 1;
};
