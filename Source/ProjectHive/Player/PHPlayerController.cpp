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
	IPHInteractableInterface* InteractActor = Cast<IPHInteractableInterface>(Target);
	if (InteractActor != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("BEGINOVERLAP"));
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

void APHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 로컬에서만 마우스 보이게 설정
	// TODO : 인게임에서는 조준점이 생기면 조준점으로 변경 예정
	ShowMouseCursor(true);
}

void APHPlayerController::ShowMouseCursor(bool IsShow)
{
	if (IsLocalController())
	{
		if (IsShow)
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);

			SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
		else
		{
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
	}
}


