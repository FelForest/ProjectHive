// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PHHUDWidget.generated.h"

/**
 * Author		: �ӵ���
 * Date			: 2025-06-16
 * Description	:
 * HUD
 * ���� ������ �޾ƿͼ� �ٲٱ�
 * ü�� ����� �ٲٱ�
 *
 */
UCLASS()
class PROJECTHIVE_API UPHHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPHHUDWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void UpdateHP(float NewHP);

	UFUNCTION()
	void UpdateGunUI(const FGunState& NewGunState);

	UFUNCTION()
	void SetupGunUI(const class APHGun* NewGun);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void PostUpdateHP();

	UFUNCTION()
	void StartUpdateHP();
protected:
	UPROPERTY()
	TObjectPtr<class URadialSlider> FrontHPSlider;

	UPROPERTY()
	TObjectPtr<class URadialSlider> BackHPSlider;

	UPROPERTY()
	FTimerHandle HpUpdateTimerHandle;

	UPROPERTY()
	FTimerHandle HpDelayTimerHandle;

	UPROPERTY()
	float TargetHp;

	UPROPERTY()
	TObjectPtr<class UTextBlock> AmmoText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> MagazineText;

	UPROPERTY()
	FString AmmoString;

	UPROPERTY()
	FString MagazineString;
};
