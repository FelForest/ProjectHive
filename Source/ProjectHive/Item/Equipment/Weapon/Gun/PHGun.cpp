// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equipment/Weapon/Gun/PHGun.h"
#include "ItemData/PHGunMontageDataAsset.h"
#include "Kismet/GameplayStatics.h"

APHGun::APHGun()
{
	WeaponType = EWeaponType::Gun;

	InitLocation = FVector(123.0f, -58.0f, -137.0f);
	EquipmentMesh->SetRelativeLocation(InitLocation);

	bCanFire = true;
	bIsReloading = false;

	// 현재 데이터 받아오는 곳이 없으므로 임의로 설정
	MaxAmmo = 10;
	FireRate = 3.0f;
	Damage = 1.0f;
	Range = 1000.0f;
	CurrentMagazine = 2;

	static ConstructorHelpers::FObjectFinder<UPHGunMontageDataAsset> GunMontageDataRef = TEXT("/Game/ProjectHive/Data/Gun/DA_ARMontage.DA_ARMontage");
	if (GunMontageDataRef.Object != nullptr)
	{
		GunMontageData = GunMontageDataRef.Object;
	}
}

void APHGun::BeginPlay()
{
	Super::BeginPlay();

	// 초기 총알 세팅
	CurrentAmmo = MaxAmmo;
}

void APHGun::Fire()
{
	// 총구 위치 받아오기
	MuzzleLocation = EquipmentMesh->GetSocketLocation(TEXT("AssaultRifleMuzzle_1"));
	MuzzleRotation = EquipmentMesh->GetSocketRotation(TEXT("AssaultRifleMuzzle_1"));

	// 히트스캔을 위한 라인트래이싱 준비
	// 시작과 끝 위치
	FVector TraceStart = MuzzleLocation;
	FVector TraceEnd = MuzzleLocation + MuzzleRotation.Vector() * Range;

	// 발사 결과를 받는 구조체
	FHitResult HitResult;

	const bool Hit = GetWorld()->LineTraceSingleByProfile(HitResult, TraceStart, TraceEnd, FName("Monster"));

	// 의문인게 이미 여기에서 맞았는지 아닌지 확인하는데
	if (Hit)
	{
		// 여기서 확인하는게 의미가 있는가에 대한 고찰
		if (HitResult.bBlockingHit)
		{
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), Damage, GetInstigatorController(), this, UDamageType::StaticClass());
		}
	}

#if WITH_EDITOR
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
#endif

}

void APHGun::ReloadStart()
{
	SetIsReloading(true);
}

void APHGun::Reload()
{
	UE_LOG(LogTemp, Log, TEXT("Reload"));
	CurrentMagazine -= 1;
	CurrentAmmo = MaxAmmo;
	// 캐릭터에서 실행하는거여서 모르겠다
	OnReload.Broadcast(CurrentAmmo, CurrentMagazine);
}

void APHGun::ReloadEnd()
{
	SetIsReloading(false);
}

void APHGun::Attack()
{
	// 상위에서는 Attack 호출
	Super::Attack();
	SetCanFire(false);
	Fire();
	float FireInterval = 1.0f / FireRate;
	// TODO : 타이머로 발사 속도에 따라 다시 발사 가능하도록 설정
	GetWorldTimerManager().SetTimer(FireRateHandle, this, &APHGun::ResetCanFire, FireInterval, false);

	CurrentAmmo -= 1;
	UE_LOG(LogTemp, Log, TEXT("%d"), CurrentAmmo);
	// 여기서 UI 업데이트 델리게이트 발행
	
}

void APHGun::DropItem(const FVector& InDropLocation)
{
	Super::DropItem(InDropLocation);

	EquipmentMesh->SetRelativeLocation(InitLocation);
}

void APHGun::OnReloadingStarted()
{
	SetIsReloading(true);

	// 현재 탄환을 버렸습니다 하게 하면 되는거라(CurrentAmmo) 넘기기

	CurrentAmmo = 0;
	// 여기서 델리게이트로 UI 업데이트
}

void APHGun::OnReloadingFinished()
{
	CurrentAmmo = MaxAmmo;

	// 여기서 델리게이트로 UI 업데이트
	SetIsReloading(false);
}

bool APHGun::CanAttack()
{
	//UE_LOG(LogTemp, Log, TEXT("a"));
	return CanFire();
}

void APHGun::SetIsReloading(bool bInIsReloading)
{
	bIsReloading = bInIsReloading;
}

void APHGun::ResetCanFire()
{
	SetCanFire(true);
}

bool APHGun::CanFire() const
{
	// 현재 탄환이 남아있고 장전중이 아니면 발사 가능
	return (CurrentAmmo > 0 && !bIsReloading && bCanFire);
}

bool APHGun::IsReloading() const
{
	return bIsReloading;
}

void APHGun::SetCanFire(bool InCanFire)
{
	bCanFire = InCanFire;
}

UPHGunMontageDataAsset* APHGun::GetGunMontageData()
{
	return GunMontageData;
}

bool APHGun::CanReload() const
{
	return (!IsReloading() && CurrentMagazine > 0);
}


