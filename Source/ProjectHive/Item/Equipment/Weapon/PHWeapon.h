// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Equipment/PHEquipment.h"
#include "Interface/PHAttackInterface.h"
#include "ItemData/PHWeaponType.h"
#include "PHWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API APHWeapon : public APHEquipment,
	public IPHAttackInterface
{
	GENERATED_BODY()

public:
	APHWeapon();

	UFUNCTION()
	virtual void Attack() override;

	UFUNCTION()
	virtual bool CanAttack();

	UFUNCTION()
	EWeaponType GetWeaponType() const;

	UFUNCTION()
	FORCEINLINE TSubclassOf<class UAnimInstance> GetWeaponAnimClass()
	{
		return WeaponAnimClass;
	}

protected:
	UPROPERTY()
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<class UAnimInstance> WeaponAnimClass;
};
