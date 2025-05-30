﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equipment/PHEquipment.h"
#include "Components/SkeletalMeshComponent.h"

APHEquipment::APHEquipment()
{
	EquipmentMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentMesh"));
	EquipmentMesh->SetupAttachment(RootComponent);
}

void APHEquipment::ResetTransform()
{
	EquipmentMesh->SetRelativeTransform(FTransform::Identity);
}

void APHEquipment::OnEquipped()
{
	ResetTransform();
}

void APHEquipment::DropItem()
{
	Super::DropItem();

	// 루트 컴포넌트랑 메시랑 맞지 않아서 조정하기 위한 위치 설정
	// TODO : 데이터로 받아와야함
	if (EquipmentType == EEquipmentType::Weapon)
	{
		EquipmentMesh->SetRelativeLocation(FVector(123.0f, -58.0f, -137.0f));
	}

	if (EquipmentType == EEquipmentType::BackPack)
	{
		EquipmentMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -98.5f));
	}
	
	SetOwner(nullptr);
	
}
