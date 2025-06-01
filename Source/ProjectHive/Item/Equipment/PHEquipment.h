// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PHItem.h"
#include "ItemData/PHEquipmentType.h"
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

	// 장비 Mesh 접근자
	FORCEINLINE USkeletalMeshComponent* GetEquipmentMesh() const
	{
		return EquipmentMesh;
	}

	UFUNCTION()
	void ResetTransform();

	UFUNCTION()
	void OnEquipped();

	UFUNCTION()
	FORCEINLINE EEquipmentType GetEquipmentType()
	{
		return EquipmentType;
	}

	UFUNCTION()
	FORCEINLINE FName GetSockName()
	{
		return SocketName;
	}

	virtual void DropItem(const FVector& InDropLocation) override;

protected:
	// 이것도 데이터를 가져와서 세팅하는 방식이 좋을거 같음
	// 장비 메시, 필요 데이터를 가지게 하는 방식으로 하는게 좋을듯
	// 하위인 무기는 기본 데이터 에셋 상속받아서 확장하는 방식이 좋을듯
	// 지금은 당장 확인하기 위한 강참조
	// 장비의 기본 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	TObjectPtr<USkeletalMeshComponent> EquipmentMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipmentype)
	EEquipmentType EquipmentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sock)
	FName SocketName;
};
