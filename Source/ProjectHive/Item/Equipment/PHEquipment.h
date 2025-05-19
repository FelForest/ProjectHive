// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PHItem.h"
#include "Interface/PHItemInterface"
#include "PHEquipment.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API APHEquipment : 
	public APHItem, 
	public IPHItemInterface
{
	GENERATED_BODY()
	
};
