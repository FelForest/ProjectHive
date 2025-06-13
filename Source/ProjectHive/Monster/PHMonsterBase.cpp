// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/PHMonsterBase.h"
#include "Components/PHMonsterSensingComponent.h"
#include "Components/PHMonsterStatComponent.h"
#include "Engine/OverlapResult.h"
#include "AI/PHMonsterController.h"

// Sets default values
APHMonsterBase::APHMonsterBase()
{
	// 감지 설정
	SensingComponent = CreateDefaultSubobject<UPHMonsterSensingComponent>(TEXT("SensingComponent"));

	// 스탯 설정
	StatComponent = CreateDefaultSubobject <UPHMonsterStatComponent>(TEXT("StatComponent"));

	// 컨트롤러 설정
	AIControllerClass = APHMonsterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void APHMonsterBase::CallAlertTarget()
{
	if (Target == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("TargtActor is nullptr"));
		return;
	}

	// 오버랩 결과
	TArray<FOverlapResult> OverlapResults;
	const FVector Position = GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GetDefaultHalfHeight() * StatComponent->GetAlertRadius());
	UE_LOG(LogTemp, Warning, TEXT("%f"), GetDefaultHalfHeight());
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel6);

	// 오버랩 시작
	bool bHit = GetWorld()->OverlapMultiByProfile(OverlapResults, Position, FQuat::Identity, TEXT("Monster"), Sphere, Params);

	UE_LOG(LogTemp, Log, TEXT("BeginOverlap"));
	if (bHit)
	{
		// 범위 안의 모든 몬스터 
		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			AActor* OverlapActor = OverlapResult.GetActor();
			if (OverlapActor == nullptr)
			{
				continue;
			}

			UE_LOG(LogTemp, Log, TEXT("Overlap hit: %s"), *OverlapActor->GetName());

			if (APHMonsterBase* NearyMonster = Cast<APHMonsterBase>(OverlapActor))
			{
				NearyMonster->SetTarget(Target);
			}
		}
	}

#if WITH_EDITOR
	DrawDebugSphere(
		GetWorld(),
		Position,
		GetDefaultHalfHeight() * StatComponent->GetAlertRadius(),
		12,                       
		FColor::Red,
		false,                    
		1.0f                      
	);
#endif
}

void APHMonsterBase::CallAlertDestination()
{

}

void APHMonsterBase::CallAlertTargetBegin(APawn* NewTarget)
{
	
	// 이미 있으면 무시
	if (Target != nullptr)	
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Alert Target"));
	SetTarget(NewTarget);
	// 몽타주 실행
	CallAlertTarget();
}

void APHMonsterBase::CallAlertDestinationBegin(FVector NewDestination)
{
	SetDestination(NewDestination);
	// 몽타주 실행
}

float APHMonsterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// 맞은 부위 쪽으로 돌려야 하니까->포인터로 받은다음-> 캐릭터 중심-> 맞은 곳 방향 구해서 돌리면 됨
	UE_LOG(LogTemp, Log, TEXT("Attacked!!"));

	if (Target != nullptr)
	{

	}
	// 이게 무거운거면 다른 방법을 사용해야함
	// Sweep 트래이스를 하던지 해서 해야하는데...-> 이건 뭐 받아온 액터와 캐릭터 앞 방향 내적때려서 시야 범위 내면 찾는 방식으로 하면 되는거니까 문제가 되는건 아님
	//SensingComponent->Scane

	return 0.0f;
}

void APHMonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SensingComponent->OnSeePawn.AddDynamic(this, &APHMonsterBase::CallAlertTargetBegin);
}

