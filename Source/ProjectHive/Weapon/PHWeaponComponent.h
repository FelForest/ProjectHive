// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData/PHWeaponType.h"
#include "PHWeaponComponent.generated.h"

class APHWeapon;
class UAnimMontage;
class APHGun;

using WeaponSetupFunc = void (UPHWeaponComponent::*)();
using WeaponRemoveDelegateFunc = void (UPHWeaponComponent::*)(class APHWeapon*);
// UI 및 스탯 컴포넌트에 사용할 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGunEquipped, const APHGun* /*EquippedWeapon*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGunUnequipped, const APHGun* /*EquippedWeapon*/);

// 무기마다 델리게이트 하나씩 더 만드는게 맞는 판단인지 모르겠다

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGunUpdate, const FGunState&);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTHIVE_API UPHWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPHWeaponComponent();

	// 캐릭터한테 공격 입력이 들어오면 실행 할 함수
	UFUNCTION()
	void Attack();

	// WeaponComponent에 바인딩할 함수
	UFUNCTION()
	void SetWeapon(class APHEquipment* InWeapon);

	UFUNCTION()
	void ClearWeapon(class APHEquipment* InWeapon);

	UFUNCTION()
	class APHWeapon* GetWeapon() const;

	UFUNCTION()
	bool CanAttack();

	// 무기(총)가 가지고 있는 몽타주 Getter 함수
	UFUNCTION()
	UAnimMontage* GetNormalAimAttackMontage();

	UFUNCTION()
	UAnimMontage* GetNormalAttackMontage();

	UFUNCTION()
	UAnimMontage* GetSpecialAttackMontage();

	UFUNCTION()
	UAnimMontage* GetReloadMontage();

	UFUNCTION()
	UAnimMontage* GetThrowMontage();

	// 총 전용 함수
	UFUNCTION()
	bool CanReload();

	UFUNCTION()
	void ReloadStart();

	UFUNCTION()
	void ReloadEnd(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void UpdateGunInfo(const FGunState& NewGunState);

	void InitializeWeaponMesh(class USkeletalMeshComponent* CharacterMesh);

public:
	FOnGunEquipped OnGunEquipped;
	FOnGunUnequipped OnGunUnequipped;

	FOnGunUpdate OnGunUpdate;

protected:
	UPROPERTY()
	// 현재 무기
	TObjectPtr<class APHWeapon> CurrentWeapon;

	// 타입 별로 캐스팅 하게 해야하는데 이거 하나씩 만드는거 별로인데 방법이 있나?
	// 일단 타입(enum) + 캐스팅(함수포인터)를 맵으로 저장해 두기

	
	TMap<EWeaponType, WeaponSetupFunc> WeaponSettingMap;
	TMap<EWeaponType, WeaponRemoveDelegateFunc> WeaponRemoveDelegateMap;


	// 캐스팅 용 함수 -> 이름을 다르게 만들어야 하나, 파라미터가 같은데.., 탬플릿으로 만들려고 해도 결국은 입력이 같으니까 의미가 없고 더 좋은 방법이 있나?
	// 이 방향이 맞나?
	// 실제 타입을 아는 것은 cpp에서만 해도 될거 같음
	void SetGun();

	// 무기 타입에 맞춰서 델리게이트 해제를 위한 함수
	void RemoveGun(class APHWeapon* InCurrentWeapon);

	// 무기(총) 몽타주
	TObjectPtr<UAnimMontage> NormalAimAttackMontage;

	TObjectPtr<UAnimMontage> NormalAttackMontage;

	TObjectPtr<UAnimMontage> SpecialAttackMontage;

	TObjectPtr<UAnimMontage> ReloadMontage;

	TObjectPtr<UAnimMontage> ThrowMontage;

};
