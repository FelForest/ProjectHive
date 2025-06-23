// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Consumable/PHGrenade.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

APHGrenade::APHGrenade()
{
	// 콜리전 생성
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));

	// 루트 설정
	SetRootComponent(CollisionComponent);
	//CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//CollisionComponent->SetSimulatePhysics(false);
	CollisionComponent->SetCollisionProfileName(TEXT("Grenade"));
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	
	// 메시 생성
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	// 루트에 붙여주기
	StaticMeshComponent->SetupAttachment(CollisionComponent);

	// 메시 내용물 채우기
	// Load GrenadeMesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("/Game/ModularSciFiHeroesPolyart/Mesh/ModularParts/Int_Grenade_SM.Int_Grenade_SM"));
	if (StaticMesh.Object)
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh.Object);
	}

	// 투사체 무브먼트 생성
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	ProjectileMovementComponent->ProjectileGravityScale = 1.0f;
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);

	// 바닥에 떨어지고 나서 수류탄 터치는데 걸리는 시간
	DelayTime = 0.5f;
}

UProjectileMovementComponent* APHGrenade::GetProjectileMovement()
{
	return ProjectileMovementComponent;
}

void APHGrenade::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Onhit 델리게이트에 함수 바인딩
	CollisionComponent->OnComponentHit.AddDynamic(this, &APHGrenade::OnGrenadeHit);
}



void APHGrenade::OnGrenadeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 자기 자신과 소유주가 오버랩 되는것을 막음 
	if (OtherActor == this || OtherActor == GetOwner())
	{
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Hit"));
	UE_LOG(LogTemp, Log, TEXT("%s"), *OtherActor->GetName());

	// 붙이치면 바로 폭발 함수 호출
	GetWorldTimerManager().SetTimer(ExplodeTimerHandle, this, &APHGrenade::Explode, DelayTime, false);

	
}

void APHGrenade::Explode()
{
	// TODO : 현재는 수류탄 종류가 하나만 있어서 여기서 생성, 추후 종류가 다양해지면 폭발액터 따로 만들어야함

	// TODO : 폭발 이팩트 재생
	
	// TODO : 폭발 사운드 재생
	
	// 데미지 주는 함수 -> 오버랩 생성
	
	UE_LOG(LogTemp, Log, TEXT("Explision"));

	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 32, FColor::Red, false, 1.0f);

	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, UDamageType::StaticClass(), /*CONSIDER : 무시해야 하는 액터들->최적화를 위한것, 벽 뒤에 있는 것들 이런건 고민 해봐야함*/ TArray<AActor*>(), this, GetInstigatorController(), true, ECC_GameTraceChannel6);

	// 수류탄 제거 -> 이펙트 끝나고 삭제되어야함
	SetLifeSpan(/*TODO : 이펙트 시간 받아와야함*/ 1.0f);
}

void APHGrenade::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UE_LOG(LogTemp, Log, TEXT("Grenade is Destroyed"));
}

USphereComponent* APHGrenade::GetCollisionComponent()
{
	return CollisionComponent;
}
