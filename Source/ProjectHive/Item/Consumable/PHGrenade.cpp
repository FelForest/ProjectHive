// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Consumable/PHGrenade.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

APHGrenade::APHGrenade()
{
	// �ݸ��� ����
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));

	// ��Ʈ ����
	SetRootComponent(CollisionComponent);
	//CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//CollisionComponent->SetSimulatePhysics(false);
	CollisionComponent->SetCollisionProfileName(TEXT("Grenade"));
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	
	// �޽� ����
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	// ��Ʈ�� �ٿ��ֱ�
	StaticMeshComponent->SetupAttachment(CollisionComponent);

	// �޽� ���빰 ä���
	// Load GrenadeMesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("/Game/ModularSciFiHeroesPolyart/Mesh/ModularParts/Int_Grenade_SM.Int_Grenade_SM"));
	if (StaticMesh.Object)
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh.Object);
	}

	// ����ü �����Ʈ ����
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	ProjectileMovementComponent->ProjectileGravityScale = 1.0f;
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
}

UProjectileMovementComponent* APHGrenade::GetProjectileMovement()
{
	return ProjectileMovementComponent;
}

void APHGrenade::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Onhit ��������Ʈ�� �Լ� ���ε�
	CollisionComponent->OnComponentHit.AddDynamic(this, &APHGrenade::OnGrenadeHit);
}



void APHGrenade::OnGrenadeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// �ڱ� �ڽŰ� �����ְ� ������ �Ǵ°��� ���� 
	if (OtherActor == this || OtherActor == GetOwner())
	{
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Hit"));
	UE_LOG(LogTemp, Log, TEXT("%s"), *OtherActor->GetName());

	// ����ġ�� �ٷ� ���� �Լ� ȣ��
	Explode();

	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 32, FColor::Red, false, 1.0f);
}

void APHGrenade::Explode()
{
	// TODO : ����� ����ź ������ �ϳ��� �־ ���⼭ ����, ���� ������ �پ������� ���߾��� ���� ��������

	// TODO : ���� ����Ʈ ���
	
	// TODO : ���� ���� ���
	
	// ������ �ִ� �Լ� -> ������ ����
	
	UE_LOG(LogTemp, Log, TEXT("Explision"));

	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, UDamageType::StaticClass(), /*CONSIDER : �����ؾ� �ϴ� ���͵�->����ȭ�� ���Ѱ�, �� �ڿ� �ִ� �͵� �̷��� ��� �غ�����*/ TArray<AActor*>(), this, GetInstigatorController(), true, ECC_Visibility);

	// ����ź ���� -> ����Ʈ ������ �����Ǿ����
	SetLifeSpan(/*TODO : ����Ʈ �ð� �޾ƿ;���*/ 1.0f);
}

void APHGrenade::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UE_LOG(LogTemp, Log, TEXT("Grenade is Destroyed"));
}
