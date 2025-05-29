// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"

#include "PHCharacterInputActionData.generated.h"

UENUM()
enum class ECharacterActionType : uint8
{
	None = 0,
	MoveAction = 1,
	InteractAction = 2
};

USTRUCT()
struct FPHInputBinding
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInputAction> InputAction;

	UPROPERTY(EditAnywhere)
	ETriggerEvent TriggerEvent;
};


/**
 * 
 */
UCLASS()
class PROJECTHIVE_API UPHCharacterInputActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPHCharacterInputActionData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InputMapping)
	TObjectPtr <class UInputMappingContext> CharacterInputMapping;

	UPROPERTY(EditAnywhere, Category = ActionBinding)
	TMap<ECharacterActionType, FPHInputBinding> InputBindings;
};
