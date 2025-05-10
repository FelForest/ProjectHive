// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PHPartsCharacter.h"
#include "PHAttackableCharacter.generated.h"

// TODO : Consider Delegate Param
DECLARE_MULTICAST_DELEGATE(FOnEquipmentAcquired);

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API APHAttackableCharacter : public APHPartsCharacter
{
	GENERATED_BODY()
	
public:
	APHAttackableCharacter();


	// Delegate Section
	void SetEquipment(/*InEquipment*/);

public:
	FOnEquipmentAcquired OnEquipmentAcquired;

protected:
	// Weapon Section
	// TODO : Request Naming Update
	TObjectPtr<class UPHWeaponComponent> Weapon;

	// Mesh Section
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
};
