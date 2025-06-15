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

	virtual void OnPossess(APawn* aPawn) override;

	UFUNCTION()
	void ShowInteractUI(class AActor* Target);

	UFUNCTION()
	void HideInteractUI(class AActor* Target);

	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	void ShowMouseCursor(bool IsShow);

	// HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UPHHUDWidget> PHHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUd)
	TObjectPtr<class UPHHUDWidget> PHHUDWidget;
};
