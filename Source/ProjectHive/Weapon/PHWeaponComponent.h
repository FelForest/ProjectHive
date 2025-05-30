﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PHWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTHIVE_API UPHWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPHWeaponComponent();

	UFUNCTION()
	void Attack();

	UFUNCTION()
	void SetWeapon(/*New WeaponData*/);

	void InitializeWeaponMesh(class USkeletalMeshComponent* CharacterMesh);
	
};
