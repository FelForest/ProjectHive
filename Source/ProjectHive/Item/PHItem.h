// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PHInteractableInterface.h"
#include "PHItem.generated.h"

/*
* Author : 임동현
* Date : 2025-05-26
* 
* Description : Item 클래스
*				기본적인 획득이 가능한 클래스
* 
*/

UCLASS()
class PROJECTHIVE_API APHItem : public AActor,
	public IPHInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APHItem();

	// 상호작용 키 활성화용
	virtual void ShowInteractUI() override;

	virtual void HideInteractUI() override;

	// 상호작용 인터페이스 구현
	virtual void Interact(class AActor* InInstigatorActor) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCapsuleComponent> ItemCollision;

	UPROPERTY()
	TObjectPtr<class UWidgetComponent> InteractWidget;
public:	
	
};
