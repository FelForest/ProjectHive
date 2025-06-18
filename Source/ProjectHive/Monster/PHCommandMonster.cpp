// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/PHCommandMonster.h"
#include "AI/PHMonsterCommandController.h"
#include "Interface/PHCommandAIInterface.h"

APHCommandMonster::APHCommandMonster()
{
	// 컨트롤러 설정
	AIControllerClass = APHMonsterCommandController::StaticClass();

	// 몽타주 에셋 로드
	/*static ConstructorHelpers::FObjectFinder<UPHMonsterMontageAsset> MonsterMontagesRef = TEXT("/Game/ProjectHive/Data/Monster/DA_MonsterBaseMontage.DA_MonsterBaseMontage");
	if (MonsterMontagesRef.Object != nullptr)
	{
		MonsterMontages = MonsterMontagesRef.Object;
	}*/

	// 하위 배열 크기 설정
	//Subordinates.Init(nullptr, OrdinateNum);
}

void APHCommandMonster::CallAlertTargetBegin(APawn* NewTarget)
{
	Super::CallAlertTargetBegin(NewTarget);

	SetIsCombat(true);

	// 하위 몬스터 한테 알려주기
	PropagateTarget(NewTarget);
}

void APHCommandMonster::PropagateTarget(APawn* NewTarget)
{
	UE_LOG(LogTemp, Warning, TEXT("PropagateTarget"));
	for (APHMonsterBase*& Subordinate : Subordinates)
	{
		if (Subordinate != nullptr)
		{
			Subordinate->SetTarget(NewTarget);
		}
	}
}

void APHCommandMonster::SetIsOnAlert(bool InIsOnAlert)
{
	CommandAI->SetIsOnAlert(InIsOnAlert);
}

void APHCommandMonster::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 매번 캐스팅 하는게 대신 빙의 한번만 할떄 하는 것으로 설정
	if (NewController->GetClass()->ImplementsInterface(UPHMonsterAIInterface::StaticClass()))
	{
		CommandAI.SetObject(NewController);
		CommandAI.SetInterface(Cast<IPHCommandAIInterface>(NewController));
	}
}

void APHCommandMonster::SetNewDestination(FVector NewDestination)
{
	SetDestination(NewDestination);
	SetIsOnAlert(true);


	for (APHMonsterBase*& Subordinate : Subordinates)
	{
		if (Subordinate != nullptr && !Subordinate->GetIsDead())
		{
			Subordinate->SetCanAlert(false);
		}
	}
}

void APHCommandMonster::OnDead()
{
	Super::OnDead();

	UE_LOG(LogTemp, Warning, TEXT("Commander Dead"));
	for (APHMonsterBase*& Subordinate : Subordinates)
	{
		if (Subordinate != nullptr)
		{
			// 원래는 다른 커맨더 세팅을 해줘야함
			Subordinate->SetCommander(nullptr);
		}
	}
}

void APHCommandMonster::SetTarget(APawn* NewTarget)
{
	Super::SetTarget(NewTarget);
	PropagateTarget(NewTarget);
}


