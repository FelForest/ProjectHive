// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PHCharacterInputActionData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API UPHCharacterInputActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPHCharacterInputActionData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InputAction)
	TObjectPtr<class UInputAction> MoveInputAction;
};
