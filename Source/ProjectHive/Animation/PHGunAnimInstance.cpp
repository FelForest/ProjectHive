// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PHGunAnimInstance.h"
#include "Character/PHPlayableCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/PHWeaponComponent.h"
#include "Item/Equipment/Weapon/Gun/PHGun.h"

UPHGunAnimInstance::UPHGunAnimInstance()
{
}

void UPHGunAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<APHPlayableCharacter>(GetOwningActor());

	if (Owner == nullptr)
	{
		return;
	}

	Movement = Owner->GetCharacterMovement();
	WeaponComponent = Owner->GetWeaponComponent();


}

void UPHGunAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 아직 못 받아 올 수도 있음
	if (Owner == nullptr)
	{
		return;
	}

	// 이동 속도 업데이트
	if (Movement != nullptr)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		// TODO : Request improvement
		bIsIdle = (GroundSpeed <= 0.0f);
	}

	// 현재 조준중인지 업데이트
	
	bIsAiming = Owner->IsAiming();
	TensionLevel = Owner->GetTensionLevel();

	if (WeaponComponent != nullptr)
	{
		Weapon = WeaponComponent->GetWeapon();
		if (Weapon != nullptr)
		{
			bIsEquipped = Weapon->IsEquipped();
		}
	}

	// 방향 계산해주는 함수 있으니 여기서 합시다

	// 현재 회전값 업데이트
	Rotation = Owner->GetActorRotation();

	// 조준방향과 이동방향의 각도 계산
	AimDirection = CalculateDirection(Velocity, Rotation);
}

void UPHGunAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
}
