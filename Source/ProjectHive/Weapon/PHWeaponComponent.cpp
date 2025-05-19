// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PHWeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UPHWeaponComponent::UPHWeaponComponent()
{
	// Need to Setting before AnimInstance Setting
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	
}

void UPHWeaponComponent::Attack()
{
}

void UPHWeaponComponent::SetWeapon()
{
	// Use Delegate to UI
}

void UPHWeaponComponent::InitializeWeaponMesh(USkeletalMeshComponent* CharacterMesh)
{
	WeaponMesh->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
	WeaponMesh->SetLeaderPoseComponent(CharacterMesh);
}

