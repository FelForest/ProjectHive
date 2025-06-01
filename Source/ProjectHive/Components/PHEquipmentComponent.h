// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData/PHEquipmentType.h"
#include "PHEquipmentComponent.generated.h"

/*	Author			: 임동현
*	Date			: 2025-05-29
*	Description		: 
	- 액터가 현재 장착 중인 장비를 관리하는 컴포넌트
	- 장비 부착 및 장착 처리 담당 (무기, 방어구 등)
	- 장비가 무기인 경우 WeaponComponent에 알림 전달
	- 장비 장착 변경 시 델리게이트를 통해 UI 등에 알림
*/

class APHEquipment;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipmentEquippedDelegate, APHEquipment*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipmentUnequippedDelegate, APHEquipment*);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTHIVE_API UPHEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPHEquipmentComponent();

	// 액터가 호출할 장비 장착 함수
	UFUNCTION()
	void Equip(class APHItem* InItem, USkeletalMeshComponent* InOwnerMesh);

	UFUNCTION()
	void DropEquipment(APHEquipment* InEquipment);

	/*UFUNCTION()
	void DropEquipment(class APHItem* InEquipment);*/

	// 무기 슬롯 결정하는 함수 필요 -> 소유주가 할거임 -> 컴포넌트 초기화 보장 이후 해줘야함
	void SetEquipmentSlot(/*EquipmentTypePreset*/);

	// 폰의 GetMesh()를 넘겨주세요
	FORCEINLINE void SetAttachMesh(class USkeletalMeshComponent* InAttachMesh)
	{
		AttachMesh = InAttachMesh;
	}

	//APHEquipment* 

public:	
	FOnEquipmentEquippedDelegate OnEquipmentEquipped;
	FOnEquipmentUnequippedDelegate OnEquipmentUnequipped;

private:
	TMap<EEquipmentType, APHEquipment*> EquipmentSlots;

	TObjectPtr<USkeletalMeshComponent> AttachMesh;
};
