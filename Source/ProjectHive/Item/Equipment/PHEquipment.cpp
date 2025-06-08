// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equipment/PHEquipment.h"
#include "Components/SkeletalMeshComponent.h"

APHEquipment::APHEquipment()
{
	ItemType = EItemType::Equipment;
	EquipmentMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentMesh"));
	EquipmentMesh->SetupAttachment(RootComponent);
	bIsEquipped = false;

}

void APHEquipment::ResetTransform()
{
	EquipmentMesh->SetRelativeTransform(FTransform::Identity);
}

void APHEquipment::OnEquipped()
{
	SetIsEquipped(true);
	ResetTransform();
	if (bIsEquipped)
	{
		UE_LOG(LogTemp, Log, TEXT("Equipped"));
	}
}

bool APHEquipment::IsEquipped() const
{
	return bIsEquipped;
}

void APHEquipment::SetIsEquipped(uint8 InIsEquipped)
{
	bIsEquipped = InIsEquipped;
}

void APHEquipment::DropItem(const FVector& InDropLocation)
{
	Super::DropItem(InDropLocation);

	// 루트 컴포넌트랑 메시랑 맞지 않아서 조정하기 위한 위치 설정
	// TODO : 데이터로 받아와야함
	if (EquipmentType == EEquipmentType::BackPack)
	{
		EquipmentMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -98.5f));
	}
	
	SetIsEquipped(false);
	SetOwner(nullptr);
	
}
