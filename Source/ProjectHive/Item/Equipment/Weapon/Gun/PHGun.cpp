// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equipment/Weapon/Gun/PHGun.h"

APHGun::APHGun()
{
	InitLocation = FVector(123.0f, -58.0f, -137.0f);
	EquipmentMesh->SetRelativeLocation(InitLocation);

	bIsReloading = false;

	// 현재 데이터 받아오는 곳이 없으므로 임의로 설정
	MaxAmmo = 10;
	FireRate = 1.0f;
	Damage = 1.0f;
	Range = 1000.0f;
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

	const bool Hit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility);

	// 의문인게 이미 여기에서 맞았는지 아닌지 확인하는데
	if (Hit)
	{
		// 여기서 확인하는게 의미가 있는가에 대한 고찰
		if (HitResult.bBlockingHit)
		{
			// 맞은 쪽에 ApplyDamage 호출
		}
	}

#if WITH_EDITOR
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
#endif

}

void APHGun::Reload()
{
	// 장전중인거 알리는 플래그값 설정
	bIsReloading = true;

	// CONSIDER : 기존 탄창에 얼마나 있는지 알려주는 방식 고려
	CurrentAmmo = 0;
	// 위에 고려시 델리게이트로 UI 업데이트 필요

}

void APHGun::Attack()
{
	// 상위에서는 Attack 호출
	Super::Attack();
	Fire();
}

void APHGun::DropItem(const FVector& InDropLocation)
{
	Super::DropItem(InDropLocation);

	EquipmentMesh->SetRelativeLocation(InitLocation);
}

void APHGun::SetIsReloading(bool bInIsReloading)
{
	bIsReloading = bInIsReloading;
}

void APHGun::FinishReload()
{
	CurrentAmmo = MaxAmmo;

	// 여기서 델리게이트로 UI 업데이트

	// 장전이 끝나다고 설정
	SetIsReloading(false);
}

bool APHGun::CanFire() const
{
	// 현재 탄환이 남아있고 장전중이 아니면 발사 가능
	return (CurrentAmmo > 0 && !bIsReloading);
}


