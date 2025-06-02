// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equipment/Weapon/Gun/PHGun.h"

APHGun::APHGun()
{
	InitLocation = FVector(123.0f, -58.0f, -137.0f);
	EquipmentMesh->SetRelativeLocation(InitLocation);

	bIsReloading = false;

	// ���� ������ �޾ƿ��� ���� �����Ƿ� ���Ƿ� ����
	MaxAmmo = 10;
	FireRate = 1.0f;
	Damage = 1.0f;
	Range = 1000.0f;
}

void APHGun::BeginPlay()
{
	Super::BeginPlay();

	// �ʱ� �Ѿ� ����
	CurrentAmmo = MaxAmmo;
}

void APHGun::Fire()
{
	// �ѱ� ��ġ �޾ƿ���
	MuzzleLocation = EquipmentMesh->GetSocketLocation(TEXT("AssaultRifleMuzzle_1"));
	MuzzleRotation = EquipmentMesh->GetSocketRotation(TEXT("AssaultRifleMuzzle_1"));

	// ��Ʈ��ĵ�� ���� ����Ʈ���̽� �غ�
	// ���۰� �� ��ġ
	FVector TraceStart = MuzzleLocation;
	FVector TraceEnd = MuzzleLocation + MuzzleRotation.Vector() * Range;

	// �߻� ����� �޴� ����ü
	FHitResult HitResult;

	const bool Hit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility);

	// �ǹ��ΰ� �̹� ���⿡�� �¾Ҵ��� �ƴ��� Ȯ���ϴµ�
	if (Hit)
	{
		// ���⼭ Ȯ���ϴ°� �ǹ̰� �ִ°��� ���� ����
		if (HitResult.bBlockingHit)
		{
			// ���� �ʿ� ApplyDamage ȣ��
		}
	}

#if WITH_EDITOR
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
#endif

}

void APHGun::Reload()
{
	// �������ΰ� �˸��� �÷��װ� ����
	bIsReloading = true;

	// CONSIDER : ���� źâ�� �󸶳� �ִ��� �˷��ִ� ��� ���
	CurrentAmmo = 0;
	// ���� ����� ��������Ʈ�� UI ������Ʈ �ʿ�

}

void APHGun::Attack()
{
	// ���������� Attack ȣ��
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

	// ���⼭ ��������Ʈ�� UI ������Ʈ

	// ������ �����ٰ� ����
	SetIsReloading(false);
}

bool APHGun::CanFire() const
{
	// ���� źȯ�� �����ְ� �������� �ƴϸ� �߻� ����
	return (CurrentAmmo > 0 && !bIsReloading);
}


