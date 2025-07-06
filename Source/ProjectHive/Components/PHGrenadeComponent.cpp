// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PHGrenadeComponent.h"
#include "Item/Consumable/PHGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPHGrenadeComponent::UPHGrenadeComponent()
{
	GrenadeClass = APHGrenade::StaticClass();
}

void UPHGrenadeComponent::ThrowGrenade(USkeletalMeshComponent* InMesh, FVector InTargetLocation)
{
	if (InMesh == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Mesh is nullptr"));
		return;
	}
	SetGrenadeData(InMesh, InTargetLocation);

	SpawnGrenade();

	if (CurrentGrenade == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("CurrentGrenade is nullptr"));
		return;
	}

	ApplyGrenadeData();
}

void UPHGrenadeComponent::SetGrenadeData(USkeletalMeshComponent* InMesh, FVector InTargetLocation)
{
	InitSpeed = 3000;
	MaxSpeed = 10000.0f;

	// 원래는 지역 변수로 하려고 했는데, 다른 함수에서도 쓰기 때문에 그냥 맴버변수 설정함
	SpawnLocation = InMesh->GetSocketLocation(TEXT("GrenadeSocket"));
	SpawnRotation = InMesh->GetSocketRotation(TEXT("GrenadeSocket"));
	TargetLocation = InTargetLocation;
}

void UPHGrenadeComponent::SpawnGrenade()
{
	FActorSpawnParameters GrenadeParms;
	// 컴포넌트의 소유자 -> 캐릭터
	GrenadeParms.Owner = GetOwner();
	// 캐릭터의 인스티게이터 -> 캐릭터 ??
	GrenadeParms.Instigator = GetOwner()->GetInstigator();

	if (GrenadeClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("grenade class is nullprt"));
		return;
	}
	
	CurrentGrenade = GetWorld()->SpawnActor<APHGrenade>(GrenadeClass, SpawnLocation, SpawnRotation, GrenadeParms);
}

void UPHGrenadeComponent::ApplyGrenadeData()
{
	// TODO : 데이터 테이블로 받아와야함
	// 타입, 데미지, 범위
	// 수류탄 데미지 및 밤위 설정

	auto Movement = CurrentGrenade->GetProjectileMovement();

	Movement->InitialSpeed = InitSpeed;
	Movement->MaxSpeed = MaxSpeed;

	FVector ForwardDir = CurrentGrenade->GetActorForwardVector();

	// 거리에 따른 속도값;
	FVector TossVelocity;

	bool bSuccess = UGameplayStatics::SuggestProjectileVelocity(
		this,
		TossVelocity,
		SpawnLocation,
		TargetLocation,
		InitSpeed,              
		false,                     
		CurrentGrenade->GetCollisionComponent()->GetScaledSphereRadius(),
		0.0f,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("Success"));
		Movement->Velocity = TossVelocity;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Fail"));
		Movement->Velocity = ForwardDir * InitSpeed + GetOwner()->GetVelocity();
	}
}

