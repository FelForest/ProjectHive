// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equipment/PHEquipment.h"
#include "Components/SkeletalMeshComponent.h"

APHEquipment::APHEquipment()
{
	EquipmentMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentMesh"));
	EquipmentMesh->SetupAttachment(RootComponent);
}
