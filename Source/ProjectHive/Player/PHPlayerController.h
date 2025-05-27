// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PHPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API APHPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APHPlayerController();

	UFUNCTION()
	void ShowInteractUI(class AActor* Target);

	UFUNCTION()
	void HideInteractUI(class AActor* Target);
};
