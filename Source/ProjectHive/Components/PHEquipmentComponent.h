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

	UFUNCTION()
	void SwapWeapon(int32 Direction);

	bool IsFullWeaponInventory();

public:	
	FOnEquipmentEquippedDelegate OnEquipmentEquipped;
	FOnEquipmentUnequippedDelegate OnEquipmentUnequipped;

private:
	// 장비 슬롯
	TMap<EEquipmentType, APHEquipment*> EquipmentSlots;

	// 폰의 Mesh를 받아오는 변수
	TObjectPtr<USkeletalMeshComponent> AttachMesh;

	// 인전하기 싫지만 무기는 따로 배열로 관리해야할듯

	// 사용할 무기 정보 배열
	UPROPERTY()
	TArray<TObjectPtr<APHEquipment>> WeaponInventory;

	// 배열 크기
	int32 WeaponInventorySize;

	// 현재 배열이 가득 찼는지 알기 위한 변수
	int32 WeaponCount;

	// 현재 배열의 몇변째 무기를 들고 있는지 알기 위한 변수
	int32 CurrentWeaponIndex;
};
