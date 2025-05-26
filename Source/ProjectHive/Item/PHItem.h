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
*				�⺻���� ȹ���� ������ 
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
	void ShowUI();

	// ��ȣ�ۿ� �������̽� ����
	virtual void Interact() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCapsuleComponent> ItemCollision;

	//UPROPERTY()
	//TObjectPtr<

public:	
	
};
