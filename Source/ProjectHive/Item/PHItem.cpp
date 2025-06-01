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
	ItemCollision->SetSimulatePhysics(true);
	ItemCollision->SetCollisionProfileName(TEXT("Interactable"));
	SetRootComponent(ItemCollision);


	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractUI"));
	InteractWidget->SetupAttachment(RootComponent);
	InteractWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));

	static ConstructorHelpers::FClassFinder<UPHInteractWidget> InteractWidgetRef(TEXT("/Game/ProjectHive/UI/WBP_Interact.WBP_Interact_C"));
	if (InteractWidgetRef.Class)
	{
		// 위젯 컴포넌트는 위젯의 클래스 정보를 바탕으로 자체적으로 인스턴스를 생성함
		InteractWidget->SetWidgetClass(InteractWidgetRef.Class);

		// 2D 모드로 그리기
		InteractWidget->SetWidgetSpace(EWidgetSpace::Screen);

		// 크기 설정
		InteractWidget->SetDrawSize(FVector2D(150.0f, 15.0f));

		// 콜리전 끄기
		InteractWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 호출시 에만 해주는걸로
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
	IPHItemInterface* InstigatorActor = Cast<IPHItemInterface>(InInstigatorActor);

	if (InstigatorActor == nullptr)
	{
		return;
	}

	ItemCollision->SetSimulatePhysics(false);
	ItemCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	InstigatorActor->PickupItem(this);
}

void APHItem::DropItem(const FVector& InDropLocation)
{
	ItemCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemCollision->SetSimulatePhysics(true);
	SetActorLocation(InDropLocation);
}

USceneComponent* APHItem::GetRootComponent()
{
	return RootComponent;
}

