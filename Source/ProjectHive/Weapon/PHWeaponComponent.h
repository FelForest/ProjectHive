// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PHWeaponComponent.generated.h"

class APHWeapon;

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

	void InitializeWeaponMesh(class USkeletalMeshComponent* CharacterMesh);


protected:
	UPROPERTY()
	// 현재 무기
	TObjectPtr<class APHWeapon> CurrentWeapon;

};
