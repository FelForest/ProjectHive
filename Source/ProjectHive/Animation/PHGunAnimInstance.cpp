// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PHGunAnimInstance.h"
#include "Character/PHPlayableCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UPHGunAnimInstance::UPHGunAnimInstance()
{
}

void UPHGunAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<APHPlayableCharacter>(GetOwningActor());

	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}


}

void UPHGunAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 이동 속도 업데이트
	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		// TODO : Request improvement
		bIsIdle = (GroundSpeed <= 0.0f);
	}

	// CONSIDER : 내적을 Instance에서 하는게 맞는지 아니면 캐릭터에서 하는게 맞는지 모르겠다

}

void UPHGunAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
}
