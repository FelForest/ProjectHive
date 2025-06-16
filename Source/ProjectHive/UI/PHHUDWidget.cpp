// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PHHUDWidget.h"
#include "Components/RadialSlider.h"
#include "Item/Equipment/Weapon/Gun/PHGun.h"

// TODO : 인터페이스로 바꾸기 필요
#include "Character/PHPlayableCharacter.h"

UPHHUDWidget::UPHHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UPHHUDWidget::UpdateHP(float NewHP)
{
	FrontHPSlider->SetValue(NewHP);

	TargetHp = NewHP;

	GetWorld()->GetTimerManager().ClearTimer(HpDelayTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(HpUpdateTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(HpDelayTimerHandle, this, &UPHHUDWidget::StartUpdateHP, 0.5f, false);
}

void UPHHUDWidget::PostUpdateHP()
{
	float CurrentHp = BackHPSlider->GetValue();
	float NewHp = FMath::FInterpTo(CurrentHp, TargetHp, 0.02f, 5.0f);
	BackHPSlider->SetValue(NewHp);

	if (FMath::IsNearlyEqual(NewHp, TargetHp, 0.01f))
	{
		BackHPSlider->SetValue(TargetHp);
		GetWorld()->GetTimerManager().ClearTimer(HpUpdateTimerHandle);
	}
}

void UPHHUDWidget::StartUpdateHP()
{
	GetWorld()->GetTimerManager().SetTimer(HpUpdateTimerHandle, this, &UPHHUDWidget::PostUpdateHP, GetWorld()->GetDeltaSeconds(), true);
}


void UPHHUDWidget::UpdateGunUI(const FGunState& NewGunState)
{
	// 받아온 총의 데이터 설정
	UE_LOG(LogTemp, Log, TEXT("GunUIUpdate"));
}

void UPHHUDWidget::SetupGunUI(const APHGun* NewGun)
{
	UE_LOG(LogTemp, Log, TEXT("SetUpGunUI"));
}

void UPHHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// HP 설정
	FrontHPSlider = Cast<URadialSlider>(GetWidgetFromName(TEXT("FrontHPSlider")));

	if (FrontHPSlider != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("FrontHPSlider is nullptr"));
	}

	BackHPSlider = Cast<URadialSlider>(GetWidgetFromName(TEXT("BackHPSlider")));

	if (BackHPSlider != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("BackHPSlider is nullptr"));
	}

	// 총 설정
	


	APHPlayableCharacter* PlayableCharacter = Cast<APHPlayableCharacter>(GetOwningPlayer()->GetPawn());



	if (PlayableCharacter != nullptr)
	{
		PlayableCharacter->SetUpHUD(this);
	}
}
