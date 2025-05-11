// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PHPartsAnimInstance.h"
#include "Character/PHPartsCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UPHPartsAnimInstance::UPHPartsAnimInstance()
{
	
}

void UPHPartsAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<APHPartsCharacter>(GetOwningActor());

	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UPHPartsAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		// TODO : Request improvement
		bIsIdle = (GroundSpeed <= 0.0f);
	}
}

void UPHPartsAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

}
