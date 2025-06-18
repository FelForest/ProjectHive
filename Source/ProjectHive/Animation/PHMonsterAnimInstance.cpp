// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PHMonsterAnimInstance.h"
#include "Monster/PHCombatMonsterBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

UPHMonsterAnimInstance::UPHMonsterAnimInstance()
{
}

void UPHMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<APHCombatMonsterBase>(GetOwningActor());

	if (Owner == nullptr)
	{
		return;
	}

	Movement = Owner->GetCharacterMovement();
}

void UPHMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	// 아직 못 받아 올 수도 있음
	if (Owner == nullptr)
	{
		return;
	}

	bIsAlerting = Owner->IsAlerting();

	// 이동 속도 업데이트
	if (Movement != nullptr)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = (GroundSpeed <= 0.0f);
	}

	
}

void UPHMonsterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{

}
