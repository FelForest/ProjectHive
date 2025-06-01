// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PHWeaponComponent.generated.h"


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
	void SetWeapon(class APHEquipment* InEquipment);

	UFUNCTION()
	void ClearWeapon(class APHEquipment* InEquipment);

	UFUNCTION()
	class APHEquipment* GetWeapon() const;


	void InitializeWeaponMesh(class USkeletalMeshComponent* CharacterMesh);

protected:
	// 사용할 무기 정보
	UPROPERTY()
	TObjectPtr<class APHWeapon> Weapon;
	
};
