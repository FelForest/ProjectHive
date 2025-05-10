// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHPartsCharacter.h"

#include "Components/CapsuleComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


APHPartsCharacter::APHPartsCharacter()
{
	// Manage data assets later
	float HalfHeight = 98.5f;

	// Setting Capsule Component
	GetCapsuleComponent()->InitCapsuleSize(34.0f, HalfHeight);

	// Setting Skeletal Mesh Parts
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -HalfHeight), FRotator(0.0f, -90.0f, 0.0f));

	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(GetMesh());
	HeadMesh->SetLeaderPoseComponent(GetMesh());

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	ArmMesh->SetupAttachment(GetMesh());
	ArmMesh->SetLeaderPoseComponent(GetMesh());

	LegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegMesh"));
	LegMesh->SetupAttachment(GetMesh());
	LegMesh->SetLeaderPoseComponent(GetMesh());


	// Setting Camera
	// TODO : Consider moving to an attackable character
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	// Update later
	// length, angle, etc...
	SpringArm->TargetArmLength = 1400.0f;
	SpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}
