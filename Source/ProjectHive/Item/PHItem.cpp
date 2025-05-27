// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PHItem.h"
#include "Components/CapsuleComponent.h"

#include "Components/WidgetComponent.h"
#include "UI/PHInteractWidget.h"

#include "Interface/PHItemInterface.h"

// Sets default values
APHItem::APHItem()
{
	ItemCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ItemCollision"));
	SetRootComponent(ItemCollision);

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractUI"));
	InteractWidget->SetupAttachment(RootComponent);
	InteractWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));

	static ConstructorHelpers::FClassFinder<UPHInteractWidget> InteractWidgetRef(TEXT("/Game/ProjectHive/UI/WBP_Interact.WBP_Interact_C"));
	if (InteractWidgetRef.Class)
	{
		UE_LOG(LogTemp, Log, TEXT("Tlqkf"));
		// ���� ������Ʈ�� ������ Ŭ���� ������ �������� ��ü������ �ν��Ͻ��� ������
		InteractWidget->SetWidgetClass(InteractWidgetRef.Class);

		// 2D ���� �׸���
		InteractWidget->SetWidgetSpace(EWidgetSpace::Screen);

		// ũ�� ����
		InteractWidget->SetDrawSize(FVector2D(150.0f, 15.0f));

		// �ݸ��� ����
		InteractWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// ȣ��� ���� ���ִ°ɷ�
	InteractWidget->SetVisibility(false);
}

void APHItem::ShowInteractUI()
{
	InteractWidget->SetVisibility(true);
}

void APHItem::HideInteractUI()
{
	InteractWidget->SetVisibility(false);
}

void APHItem::Interact(AActor* InInstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Begin ItemInteract"));
	IPHItemInterface* InstigatorActor = Cast<IPHItemInterface>(InInstigatorActor);

	if (InstigatorActor == nullptr)
	{
		return;
	}

	InstigatorActor->PickupItem(this);

	ItemCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

