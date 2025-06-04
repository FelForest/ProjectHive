// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PHEquipmentComponent.h"
#include "Item/Equipment/PHEquipment.h"
#include "Item/Equipment/Weapon/PHWeapon.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UPHEquipmentComponent::UPHEquipmentComponent()
{
	WeaponInventorySize = 2;
	WeaponInventory.Init(nullptr, WeaponInventorySize);

	CurrentWeaponIndex = 0;
	WeaponCount = 0;
}

void UPHEquipmentComponent::Equip(APHItem* InItem, USkeletalMeshComponent* InOwnerMesh)
{
	// 장비 타입을 보고 캐스팅 필요 -> 지금은 무기만 있으니 크게 상관 없음
	// 만약 무기면 무기 컴포넌트에 알려야함 -> 이것도 Modifier인게 델리게이트인가
	// 일단 장비를 원하는 소켓에 장작 -> 메시 필요
	// 이미 장비가 있으면 기존 장비 버리기 -> 버리는 함수 필요, 기존에 있는지 없는지 확인하는게 필요
	// 장비의 스탯을 받아와서 세팅 -> 스탯 컴포넌트에 알려줘야함 -> Modifier로 직접 알려주라는데 이런게 있구나 -> 델리게이트로 파라미터 넘겨도 되는거 아닌가?
	// 무기 장착이 완료 되면 UI에 알리기 -> 델리게이트

	// enum으로 타입 알고 있음 -> 캐스팅 필요 없음
	// 현재 장비차에 이미 있으면 버리기
	// 델리게이트 -> UI
	// 현재 장비를 장착
	// 델리게이트 -> UI
	// 장비의 스텟 -> 스탯 컴포넌트에 알리기

	APHEquipment* InEquipment = Cast<APHEquipment>(InItem);
	if (InEquipment == nullptr)
	{
		// 이건 장비가 아니라는거지
		return;
	}

	EEquipmentType TargetTyep = InEquipment->GetEquipmentType();
	FName SockName = InEquipment->GetSockName();

	// 슬롯에 없는 것이 들어왔을때
	if (!EquipmentSlots.Contains(TargetTyep))
	{
		// 이때 그냥 인벤토리에 들어가던가
		// UI로 알려주는 방식

		//InEquipment->DropItem(InEquipment->GetActorLocation());
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Equip : %s"), *InEquipment->GetName());
	
	// 무기는 특별 취급 해야하기 때문에 여기서 분기
	if (TargetTyep == EEquipmentType::Weapon)
	{
		// 델리게이트로 하려니까 그게 어떤 상황인지 알 방법이 없네
		// 오너 캐스팅 하고 나서 GetWeaponComponent 한다음에 넘겨주는 방식으로 하는게 맞는듯
		if (IsFullWeaponInventory())
		{
			DropEquipment(EquipmentSlots[TargetTyep]);
		}
		else
		{
			// 현재 무기가 하나 이상이면
			if (WeaponCount > 0)
			{
				// 기존 장비의 애니메이션 설정을 위한것
				WeaponInventory[CurrentWeaponIndex]->SetIsEquipped(false);

				CurrentWeaponIndex = (CurrentWeaponIndex + 1) % WeaponInventorySize;
				EquipmentSlots[TargetTyep] = nullptr;
			}
		}
	}

	// 이미 장비가 있다면 기존 장비 버리기
	if (EquipmentSlots[TargetTyep] != nullptr)
	{
		// 버리기 함수

		// 버리는 함수에서 타입별로 확인하고 분기해 주는게 맞는듯
		// 이때 델리게이트 UI

		DropEquipment(EquipmentSlots[TargetTyep]);
	}

	

	// 장착 할 수 없는 장비의 경우 막아야함 -> 인벤토리로 넘기던지, 아에 못먹게 UI로 띄우던지
	// 장비 슬롯에 장착
	EquipmentSlots[TargetTyep] = InEquipment;

	// 델리게이트 UI

	if (TargetTyep == EEquipmentType::Weapon)
	{
		// Consider : 델리게이트 넘겨준다.. 걍 변수로 들고 있어서 직접 넘겨준다..
		// 인벤토리에 무기 등록
		WeaponInventory[CurrentWeaponIndex] = InEquipment;
		WeaponCount += 1;
	}

	// 장비 장착 -> 장비한테 여기에 붙으라고 하는 방식이 맞는건가?
	InEquipment->SetOwner(GetOwner());
	InEquipment->AttachToComponent(InOwnerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SockName);
	InEquipment->GetEquipmentMesh()->SetLeaderPoseComponent(InOwnerMesh);
	InEquipment->OnEquipped();

	UE_LOG(LogTemp, Log, TEXT("Count : %d"), WeaponCount);
	UE_LOG(LogTemp, Log, TEXT("Index : %d"), CurrentWeaponIndex);

	// UI 및 스탯 갱신용 델리게이트 발행
	OnEquipmentEquipped.Broadcast(InEquipment);
}

void UPHEquipmentComponent::DropEquipment(APHEquipment* InEquipment)
{
	InEquipment->GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	InEquipment->GetEquipmentMesh()->SetLeaderPoseComponent(nullptr);
	InEquipment->GetEquipmentMesh()->SetAnimInstanceClass(nullptr);

	// TODO : 임시로 떨어지는 위치 설정, 변수로 승격 필요
	FVector DropLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 200.0f;
	InEquipment->DropItem(DropLocation);
	// 기존 장비 해제
	EquipmentSlots[InEquipment->GetEquipmentType()] = nullptr;

	if (InEquipment->GetEquipmentType() == EEquipmentType::Weapon)
	{
		WeaponCount -= 1;
		WeaponInventory[CurrentWeaponIndex] = nullptr;
	}
	
	// UI 및 스탯 갱신용 델리게이트 발행
	// 장비를 넘겨주는 이유는 UI가 어떠한 장비를 해제해야하는지 알아야 하기 때문
	OnEquipmentUnequipped.Broadcast(InEquipment);

}

//void UPHEquipmentComponent::DropEquipment(APHItem* InEquipment)
//{
//	APHEquipment* DroppedEquipment = Cast<APHEquipment>(InEquipment);
//	if (DroppedEquipment != nullptr)
//	{
//		DropEquipment(DroppedEquipment);
//	}
//}

//void UPHEquipmentComponent::DropEquipment(APHItem* InEquipment)
//{
//}

// Consider : 지금은 하나의 타입만 있어서 이런데 -> Preset 만들고 나면 분기 처리 해야함
void UPHEquipmentComponent::SetEquipmentSlot(/*EquipmentTypePreset*/)
{
	for (int32 ix = 1; ix < static_cast<int32>(EEquipmentType::End); ++ix)
	{
		EEquipmentType EquipmentType = static_cast<EEquipmentType>(ix);
		EquipmentSlots.Add(EquipmentType, nullptr);
	}
}

void UPHEquipmentComponent::SwapWeapon(int32 Direction)
{
	int32 NextWeaponIndex = (CurrentWeaponIndex + Direction + WeaponInventorySize) % WeaponInventorySize;

	if (WeaponInventory[CurrentWeaponIndex] != nullptr)
	{
		// 기존 무기 애니메이션 관련 변수 세팅
		WeaponInventory[CurrentWeaponIndex]->SetIsEquipped(false);
	}

	// 현재 슬롯에 인벤토리에 있는 무기 등록
	EquipmentSlots[EEquipmentType::Weapon] = WeaponInventory[NextWeaponIndex];
	// UI 및 스탯 갱신용 델리게이트 발행
	OnEquipmentEquipped.Broadcast(WeaponInventory[NextWeaponIndex]);
	if (EquipmentSlots[EEquipmentType::Weapon] != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("CurrentWeapon : %s"), *EquipmentSlots[EEquipmentType::Weapon]->GetName());
	}
	CurrentWeaponIndex = NextWeaponIndex;
}

bool UPHEquipmentComponent::IsFullWeaponInventory()
{
	return WeaponCount == WeaponInventorySize;
}

//void UPHEquipmentComponent::SetDropLocation(const FVector& InDropLocation)
//{
//	DropLocation = InDropLocation;
//}


