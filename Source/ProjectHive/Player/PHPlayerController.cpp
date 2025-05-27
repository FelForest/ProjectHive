// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PHPlayerController.h"
#include "Interface/PHInteractableInterface.h"

APHPlayerController::APHPlayerController()
{
}

void APHPlayerController::ShowInteractUI(AActor* Target)
{
	// TODO : ��Ƽ�� ����Ѱ�
	// �������� Ȯ��
	// �޾ƿ� ���� Interactabel���� Ȯ��
	// UI ȣ��

	IPHInteractableInterface* InteractActor = Cast<IPHInteractableInterface>(Target);
	if (InteractActor != nullptr)
	{
		InteractActor->ShowInteractUI();
	}
}

void APHPlayerController::HideInteractUI(AActor* Target)
{
	IPHInteractableInterface* InteractActor = Cast<IPHInteractableInterface>(Target);
	if (InteractActor != nullptr)
	{
		InteractActor->HideInteractUI();
	}
}


