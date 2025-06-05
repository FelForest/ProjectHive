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
	// 히스트캔으로 확인하기
	virtual void Fire() override;

	// 장전 함수
	// CONSIDER : 빠른 장전 고려
	virtual void Reload() override;

	// 상위 클래스에서 오버라이딩 하는 함수 섹션
	virtual void Attack() override;

	virtual void DropItem(const FVector& InDropLocation) override;

	// 여기서 만들어진 함수 섹션
	// 몽타주에서 노티파이로 호출하기 위한 함수
	UFUNCTION()
	void OnReloadingStarted();

	// 애니메이션 노티파이에서 실행할 함수
	UFUNCTION()
	void OnReloadingFinished();

	// 발사 여부를 알 수 있는 함수
	UFUNCTION()
	bool CanFire() const;

	UFUNCTION()
	bool IsReloading() const;

protected:
	void SetIsReloading(bool bInIsReloading);
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InitLocation)
	FVector InitLocation;

	// TODO : 데이터 에셋으로 받거나 테이블로 받거나
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
	uint8 bIsReloading : 1;

	UPROPERTY()
	FVector MuzzleLocation;

	UPROPERTY()
	FRotator MuzzleRotation;
};
