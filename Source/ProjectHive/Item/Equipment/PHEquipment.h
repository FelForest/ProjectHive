// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PHItem.h"
#include "PHEquipment.generated.h"

/**
 * 
 */

class USkeletalMeshComponent;

UCLASS()
class PROJECTHIVE_API APHEquipment : public APHItem
{
	GENERATED_BODY()
	
public:
	APHEquipment();

	// ��� Mesh ������
	FORCEINLINE USkeletalMeshComponent* GetEquipmentMesh() const
	{
		return EquipmentMesh;
	}

	UFUNCTION()
	void OnPickup();

protected:
	// �̰͵� �����͸� �����ͼ� �����ϴ� ����� ������ ����
	// ��� �޽�, �ʿ� �����͸� ������ �ϴ� ������� �ϴ°� ������
	// ������ ����� �⺻ ������ ���� ��ӹ޾Ƽ� Ȯ���ϴ� ����� ������
	// ������ ���� Ȯ���ϱ� ���� ������
	// ����� �⺻ �޽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<USkeletalMeshComponent> EquipmentMesh;
};
