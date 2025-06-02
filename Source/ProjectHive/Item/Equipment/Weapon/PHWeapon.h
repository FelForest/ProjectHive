// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Equipment/PHEquipment.h"
#include "Interface/PHAttackInterface.h"
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
};
