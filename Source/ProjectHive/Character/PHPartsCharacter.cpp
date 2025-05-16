// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHPartsCharacter.h"

#include "Components/CapsuleComponent.h"

#include "Data/PHCharacterSkeletalMeshData.h"


APHPartsCharacter::APHPartsCharacter()
{
	// Manage data assets later
	float HalfHeight = 98.5f;

	// Setting Capsule Component
	GetCapsuleComponent()->InitCapsuleSize(34.0f, HalfHeight);

	// Setting Skeletal Mesh Parts
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -HalfHeight), FRotator(0.0f, -90.0f, 0.0f));

	// TODO : Load SkeletalMesh for customizing

	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(GetMesh());
	HeadMesh->SetLeaderPoseComponent(GetMesh());

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	ArmMesh->SetupAttachment(GetMesh());
	ArmMesh->SetLeaderPoseComponent(GetMesh());

	LegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegMesh"));
	LegMesh->SetupAttachment(GetMesh());
	LegMesh->SetLeaderPoseComponent(GetMesh());

	// Setting SkeletalMesh
	static ConstructorHelpers::FObjectFinder<UPHCharacterSkeletalMeshData> CharacterParts(TEXT("/Game/ProjectHive/Data/Character/PHC_PartsData.PHC_PartsData"));
	if (CharacterParts.Object != nullptr)
	{
		PartsMeshData = CharacterParts.Object;

		SetCharacterPartsMesh();
	}

}

void APHPartsCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterPartsMesh();
}

void APHPartsCharacter::SetCharacterPartsMesh()
{
	USkeletalMesh* Head = PartsMeshData->HeadMesh;
	USkeletalMesh* Body = PartsMeshData->BodyMesh;
	USkeletalMesh* Arm = PartsMeshData->ArmMesh;
	USkeletalMesh* Leg = PartsMeshData->LegMesh;

	if (Body != nullptr)
	{
		GetMesh()->SetSkeletalMesh(Body);
	}
	if (Head != nullptr)
	{
		HeadMesh->SetSkeletalMesh(Head);
	}
	if (Arm != nullptr)
	{
		ArmMesh->SetSkeletalMesh(Arm);
	}
	if (Leg != nullptr)
	{
		LegMesh->SetSkeletalMesh(Leg);
	}
}
