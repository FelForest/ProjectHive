// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Equipment/Weapon/PHWeapon.h"
#include "Interface/PHFireInterface.h"
#include "Interface/PHReloadInterface.h"
#include "PHGun.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API APHGun : public APHWeapon,
	public IPHFireInterface,
	public IPHReloadInterface
{
	GENERATED_BODY()
	
public:
	APHGun();

	virtual void BeginPlay() override;

	// Interface Section
	// ����Ʈĵ���� Ȯ���ϱ�
	virtual void Fire() override;

	// ���� �Լ�
	// CONSIDER : ���� ���� ���
	virtual void Reload() override;

	// ���� Ŭ�������� �������̵� �ϴ� �Լ� ����
	virtual void Attack() override;

	virtual void DropItem(const FVector& InDropLocation) override;

	// ���⼭ ������� �Լ� ����
	// ��Ÿ�ֿ��� ��Ƽ���̷� ȣ���ϱ� ���� �Լ�
	UFUNCTION()
	void SetIsReloading(bool InIsReloading);

	// �ִϸ��̼� ��Ƽ���̿��� ������ �Լ�
	UFUNCTION()
	void FinishReload();

protected:
	// �߻� ���θ� �� �� �ִ� �Լ�
	bool CanFire() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InitLocation)
	FVector InitLocation;

	// TODO : ������ �������� �ްų� ���̺�� �ްų�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunData)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunData)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunData)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunData)
	float Range;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunData)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reload)
	uint8 bIsReloading;

	UPROPERTY()
	FVector MuzzleLocation;

	UPROPERTY()
	FRotator MuzzleRotation;
};
