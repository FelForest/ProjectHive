// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/PHMonsterBase.h"
#include "Components/PHMonsterSensingComponent.h"
#include "Components/PHMonsterStatComponent.h"

// Sets default values
APHMonsterBase::APHMonsterBase()
{
	// 감지 설정
	SensingComponent = CreateDefaultSubobject<UPHMonsterSensingComponent>(TEXT("SensingComponent"));

	// 스탯 설정
	StatComponent = CreateDefaultSubobject <UPHMonsterStatComponent>(TEXT("StatComponent"));
}

void APHMonsterBase::CallAlertTarget()
{
	if (Target == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("TargtActor is nullptr"));
		return;
	}

	// 오버랩 결과
	TArray<FHitResult> HitResults;
	const FVector Position = GetActorLocation();
	FCollisionShape Shpere = FCollisionShape::MakeSphere(StatComponent->GetAlertRadius());
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// 오버랩 시작
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, Position, Position, FQuat::Identity, ECC_GameTraceChannel6, Shpere, Params);

	if (bHit)
	{
		// 모든 액터들 돌면서 몬스터이면 target 변경시키기
		for (const FHitResult& HitResult : HitResults)
		{
			APHMonsterBase* NearyMonster = Cast<APHMonsterBase>(HitResult.GetActor());
			NearyMonster->SetTarget(Target);
		}
	}
}

void APHMonsterBase::CallAlertDestination()
{

}

void APHMonsterBase::CallAlertBegin(AActor* NewTarget)
{
	if (NewTarget == nullptr)
	{
		return;
	}

	SetTarget(NewTarget);
	// 몽타주 실행
}

void APHMonsterBase::CallAlertBegin(FVector NewDestination)
{
	SetDestination(NewDestination);
	// 몽타주 실행
}

float APHMonsterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Log, TEXT("Attacked!!"));

	return 0.0f;
}
