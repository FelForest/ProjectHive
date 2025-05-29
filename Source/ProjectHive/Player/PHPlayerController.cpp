// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PHPlayerController.h"
#include "Interface/PHInteractableInterface.h"

APHPlayerController::APHPlayerController()
{
}

void APHPlayerController::ShowInteractUI(AActor* Target)
{
	// TODO : 멀티를 고려한것
	// 로컬인지 확인
	// 받아온 액터 Interactabel인지 확인
	// UI 호출
	if (!IsLocalController())
	{
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Only Client"));
	IPHInteractableInterface* InteractActor = Cast<IPHInteractableInterface>(Target);
	if (InteractActor != nullptr)
	{
		InteractActor->ShowInteractUI();
	}
}

void APHPlayerController::HideInteractUI(AActor* Target)
{
	if (!IsLocalController())
	{
		return;
	}

	IPHInteractableInterface* InteractActor = Cast<IPHInteractableInterface>(Target);
	if (InteractActor != nullptr)
	{
		InteractActor->HideInteractUI();
	}
}


