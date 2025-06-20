// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/PHMonsterBase.h"
#include "Components/PHMonsterSensingComponent.h"
#include "Components/PHMonsterStatComponent.h"
#include "Engine/OverlapResult.h"
#include "AI/PHMonsterController.h"
#include "Interface/PHSensingAIInterface.h"
#include "AI/PHMonsterAIInterface.h"
#include "Interface/PHCommandMonsterInterface.h"


#include "Data/Monster/PHMonsterMontageAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	// 몽타주 에셋 로드
	static ConstructorHelpers::FObjectFinder<UPHMonsterMontageAsset> MonsterMontagesRef = TEXT("/Game/ProjectHive/Data/Monster/DA_MonsterBaseMontage.DA_MonsterBaseMontage");
	if (MonsterMontagesRef.Object != nullptr)
	{
		MonsterMontages = MonsterMontagesRef.Object;
	}

	// 현재 전투중인지 확인하는 변수
	bIsInCombat = false;

	// 현재 죽었는지 아닌지 확인하는 변수
	bIsDead = false;

	// 경고 시간 설정
	AlertTime = 1.0f;

	// 경고 중인지 아닌지 애니메이션 설정 값을 위한 변수
	bIsAlerting = false;
}

void APHMonsterBase::CallAlertTarget()
{
	if (GetTarget() == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("TargtActor is nullptr"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("CallAlertTarget"));
	// 오버랩 결과
	TArray<FOverlapResult> OverlapResults;
	const FVector Position = GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GetDefaultHalfHeight() * StatComponent->GetAlertRadius());
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel6);

	// 오버랩 시작
	bool bHit = GetWorld()->OverlapMultiByProfile(OverlapResults, Position, FQuat::Identity, TEXT("Monster"), Sphere, Params);

	
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

			//UE_LOG(LogTemp, Log, TEXT("Overlap hit: %s"), *OverlapActor->GetName());

			if (APHMonsterBase* NearyMonster = Cast<APHMonsterBase>(OverlapActor))
			{
				NearyMonster->SetCanAlert(false);
				NearyMonster->SetIsCombat(true);
				NearyMonster->SetTarget(GetTarget());
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


	CallAlertTargetEnd();
}

void APHMonsterBase::CallAlertDestination()
{
	// TODO : Commender있으면 알려주기
	UE_LOG(LogTemp, Log, TEXT("CallAlertDestination"));
	UE_LOG(LogTemp, Log, TEXT("NewDestination : %f , %f"), Destination.X, Destination.Y);
	if (Commander != nullptr && !Commander->GetIsDead())
	{
		IPHCommandMonsterInterface* InCommander = Cast<IPHCommandMonsterInterface>(Commander);
		if (InCommander != nullptr)
		{
			InCommander->SetNewDestination(Destination);
		}
	}
}

void APHMonsterBase::CallAlertTargetBegin(APawn* NewTarget)
{
	if (NewTarget == nullptr)
	{
		return;
	}

	// 이미 있으면 무시
	if (GetTarget() != nullptr)	
	{
		return;
	}

	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("AnimInstance is nullptr"));
		return;
	}


	UE_LOG(LogTemp, Log, TEXT("CallAlertTargetBegin"));
	// 상위 계층에게 현재 타겟 위치 알려주기-> 모든 객체는 하위에서 상위로 전달시 target자체를 넘길 수 없음
	// 상위 -> 하위이면 target설정이 가능,

	SetTarget(NewTarget);

	if (Commander != nullptr && !Commander->GetIsCombat())
	{
		SetCanAlert(true);
	}

	CallAlertTarget();

	if (MonsterMontages->BeginAlertMontage == nullptr)
	{
		return;
	}
}

void APHMonsterBase::CallAlertDestinationBegin(APawn* NewTarget)
{
	FVector NewDestination;
	if (NewTarget == nullptr)
	{
		NewDestination = GetActorLocation();
	}
	else
	{
		NewDestination = NewTarget->GetActorLocation();
	}

	SetDestination(NewDestination);

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APHMonsterBase::CallAlertDestinationEnd);
	AnimInstance->Montage_Play(MonsterMontages->BeginAlertMontage, 1.0f);

	AnimInstance->Montage_SetEndDelegate(EndDelegate, MonsterMontages->BeginAlertMontage);
	bIsAlerting = true;
	UE_LOG(LogTemp, Log, TEXT("CallAlertDestinationBegin"));
}

void APHMonsterBase::CallAlertTargetEnd()
{
	UE_LOG(LogTemp, Log, TEXT("CallAlertTargetEnd"));
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	SensingComponent->SetActive(false);
}

void APHMonsterBase::CallAlertDestinationEnd(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Log, TEXT("CallAlertDestinationEnd"));
	FTimerHandle AlertTimerHandle;

	GetWorldTimerManager().SetTimer(
		AlertTimerHandle,
		FTimerDelegate::CreateLambda([this]()
			{
				if (!AnimInstance->Montage_IsPlaying(MonsterMontages->EndAlertMontage))
				{
					AnimInstance->Montage_Play(MonsterMontages->EndAlertMontage, -1.0f);
					SetIsAlerting(false);
					SetIsCombat(true);
					GetCharacterMovement()->SetMovementMode(MOVE_Walking);
					SetCanAlert(false);

					OnAlertFinished.ExecuteIfBound();
				}
			}),
		AlertTime,
		false
	);
}

bool APHMonsterBase::IsAlerting()
{
	return bIsAlerting;
}

void APHMonsterBase::SetIsAlerting(bool InIsAlerting)
{
	bIsAlerting = InIsAlerting;
}

void APHMonsterBase::SetMonsterAlertDelegate(const FMonsterAlertFinished& InOnAlertFinished)
{
	OnAlertFinished = InOnAlertFinished;
}

void APHMonsterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 매번 캐스팅 하는게 대신 빙의 한번만 할떄 하는 것으로 설정
	if (NewController->GetClass()->ImplementsInterface(UPHSensingAIInterface::StaticClass()))
	{
		SensingAI.SetObject(NewController);
		SensingAI.SetInterface(Cast<IPHSensingAIInterface>(NewController));
	}

	// 매번 캐스팅 하는게 대신 빙의 한번만 할떄 하는 것으로 설정
	if (NewController->GetClass()->ImplementsInterface(UPHMonsterAIInterface::StaticClass()))
	{
		MonsterAI.SetObject(NewController);
		MonsterAI.SetInterface(Cast<IPHMonsterAIInterface>(NewController));
	}
}

void APHMonsterBase::SetTarget(APawn* NewTarget)
{
	//UE_LOG(LogTemp, Log, TEXT("Set Target"));
	if (SensingAI == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("SensingAI is nullptr"));
		return;
	}

	SetIsCombat((NewTarget != nullptr));
	SensingAI->SetTarget(NewTarget);
}

APawn* APHMonsterBase::GetTarget() const
{
	if (SensingAI == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("SensingAI is nullptr"));
		return nullptr;
	}
	return SensingAI->GetTarget();
}

void APHMonsterBase::SetDestination(FVector NewDestination)
{
	if (SensingAI == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("SensingAI is nullptr"));
		return;
	}
	SensingAI->SetDestination(NewDestination);
	Destination = NewDestination;
}

float APHMonsterBase::GetAttackRange() const
{
	return StatComponent->GetAttackRange();
}

FVector APHMonsterBase::GetDestination() const
{
	if (SensingAI == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("SensingAI is nullptr"));
		return FVector();
	}
	return SensingAI->GetDestination();
}

void APHMonsterBase::SetIsCombat(bool NewIsCombat)
{
	if (MonsterAI)
	{
		MonsterAI->SetIsCombat(NewIsCombat);
		bIsInCombat = NewIsCombat;
	}
}

bool APHMonsterBase::GetIsCombat()
{
	return bIsInCombat;
}

void APHMonsterBase::SetCanAlert(bool NewCanAlert)
{
	if (MonsterAI == nullptr)
	{
		return;
	}

	MonsterAI->SetCanAlert(NewCanAlert);

	if (!NewCanAlert && Commander != nullptr && !Commander->GetIsCombat())
	{
		GetWorldTimerManager().ClearTimer(AlertTimer);
		GetWorldTimerManager().SetTimer(
			AlertTimer,
			this,
			&APHMonsterBase::ResetCanALert,
			5.0f,
			false);
	}
}

void APHMonsterBase::ResetCanALert()
{
	MonsterAI->SetCanAlert(true);
}

float APHMonsterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// 맞은 부위 쪽으로 돌려야 하니까->포인터로 받은다음-> 캐릭터 중심-> 맞은 곳 방향 구해서 돌리면 됨
	UE_LOG(LogTemp, Log, TEXT("Attacked!!"));

	// 타겟 설정
	CallAlertTargetBegin(EventInstigator->GetPawn());

	if (GetTarget() != nullptr)
	{

	}
	// 이게 무거운거면 다른 방법을 사용해야함
	// Sweep 트래이스를 하던지 해서 해야하는데...-> 이건 뭐 받아온 액터와 캐릭터 앞 방향 내적때려서 시야 범위 내면 찾는 방식으로 하면 되는거니까 문제가 되는건 아님
	//SensingComponent->Scane
	
	// 현재 전투중으로 변경
	bIsInCombat = true;

	StatComponent->ChangeHP(Damage);

	return 0.0f;
}

void APHMonsterBase::OnDead()
{
	if (bIsDead)
	{
		return;
	}
	bIsDead = true;

	// TODO : 죽으면 다른 행동 못하도록 막아야함
	MonsterAI->StopAI();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	if (AnimInstance != nullptr && MonsterMontages->DieMontage != nullptr)
	{
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &APHMonsterBase::FinializeDeath);
		
		AnimInstance->Montage_Play(MonsterMontages->DieMontage, 1.0f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, MonsterMontages->DieMontage);
	}


}

bool APHMonsterBase::GetIsDead() const
{
	return bIsDead;
}

void APHMonsterBase::FinializeDeath(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Log, TEXT("Destroy"));
	Destroy();
}

void APHMonsterBase::SetCommander(APHMonsterBase* NewCommander)
{
	Commander = NewCommander;
}

APHMonsterBase* APHMonsterBase::GetCommander()
{
	return Commander;
}


void APHMonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 컴포넌트가 초기화 되고 설정
	AnimInstance = GetMesh()->GetAnimInstance();

	// 찾았을때 주위에 알리기 위한 함수 바인딩
	SensingComponent->OnSeePawn.AddDynamic(this, &APHMonsterBase::CallAlertTargetBegin);

	// 스탯 설정
	GetCharacterMovement()->MaxWalkSpeed = StatComponent->GetMoveSpeed();

	// 데미지 받았을때 죽었는지 확인하는 바인딩
	StatComponent->OnDead.AddUObject(this, &APHMonsterBase::OnDead);
}

