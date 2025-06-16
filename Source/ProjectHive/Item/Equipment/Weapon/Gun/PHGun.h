// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Equipment/Weapon/PHWeapon.h"
#include "Interface/PHFireInterface.h"
#include "Interface/PHReloadInterface.h"
#include "PHGun.generated.h"

/**
 * Author		: 임동현
 * Date			: 2025-06-02
 * Description	:
 * 총 액터
 * 기본 발사시 총구에서 앞 방향으로 레이케스트를 쏘아서 히트스캔을 함
 * 탄환, 탄창, 관리
 * 발사 및 재장전시 델리게이트 발행
 */


// UI 업데이트를 위한 구조체
USTRUCT(BlueprintType)
struct FGunState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunData)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunData)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunData)
	int32 MaxMagazine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunData)
	int32 CurrentMagazine;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGunUpdate, const FGunState&);

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
	
	// 무기 컴포넌트에서 실행할 함수
	virtual void ReloadStart() override;

	// CONSIDER : 빠른 장전 고려
	virtual void Reload() override;

	virtual void ReloadEnd() override;

	virtual bool IsReloading() const override;

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

	virtual bool CanAttack() override; 

	// 발사 여부를 알 수 있는 함수
	UFUNCTION()
	bool CanFire() const;

	UFUNCTION()
	void SetCanFire(bool InCanFire);

	UFUNCTION()
	class UPHGunMontageDataAsset* GetGunMontageData();

	UFUNCTION()
	virtual bool CanReload() const override;

	UFUNCTION()
	FGunState GetGunState() const;

protected:
	void SetIsReloading(bool bInIsReloading);

	UFUNCTION()
	void ResetCanFire();

public:
	FOnGunUpdate OnGunUpdate;

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

	UPROPERTY()
	uint8 bCanFire : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunData)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunData)
	int32 MaxMagazine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunData)
	int32 CurrentMagazine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reload)
	uint8 bIsReloading : 1;

	UPROPERTY()
	FVector MuzzleLocation;

	UPROPERTY()
	FRotator MuzzleRotation;

	// 발사 속도를 조절하는 타이머 핸들
	UPROPERTY()
	FTimerHandle FireRateHandle;


	// 무기마다 몽타주가 다르기 때문에 무기에서 관리하는 방식으로 설정 -> 추후 데이터 에셋으로 불러오게 만들예정
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = Montage, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UPHGunMontageDataAsset> GunMontageData;

};
