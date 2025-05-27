// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PHInteractableInterface.h"
#include "PHItem.generated.h"

/*
* Author : �ӵ���
* Date : 2025-05-26
* 
* Description : Item Ŭ����
*				�⺻���� ȹ���� ������ Ŭ����
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

	// ��ȣ�ۿ� Ű Ȱ��ȭ��
	virtual void ShowInteractUI() override;

	virtual void HideInteractUI() override;

	// ��ȣ�ۿ� �������̽� ����
	virtual void Interact(class AActor* InInstigatorActor) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCapsuleComponent> ItemCollision;

	UPROPERTY()
	TObjectPtr<class UWidgetComponent> InteractWidget;
public:	
	
};
