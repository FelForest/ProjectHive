// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/PHScoutMonster.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/PHCommandMonsterInterface.h"
#include "Data/Monster/PHMonsterMontageAsset.h"
#include "Interface/PHScoutAIInterface.h"

void APHScoutMonster::SetMonsterAlertDelegate(const FMonsterAlertFinished& InOnAlertFinished)
{
}

void APHScoutMonster::SetDestination(FVector NewDestination)
{
	Destination = NewDestination;
}

void APHScoutMonster::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 매번 캐스팅 하는게 대신 빙의 한번만 할떄 하는 것으로 설정
	if (NewController->GetClass()->ImplementsInterface(UPHScoutAIInterface::StaticClass()))
	{
		ScoutAI.SetObject(NewController);
		ScoutAI.SetInterface(Cast<IPHScoutAIInterface>(NewController));
	}
}

APHScoutMonster::APHScoutMonster()
{

}

void APHScoutMonster::CallAlertDestination()
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

void APHScoutMonster::CallAlertDestinationBegin(APawn* NewTarget)
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
	EndDelegate.BindUObject(this, &APHScoutMonster::CallAlertDestinationEnd);
	AnimInstance->Montage_Play(MonsterMontages->BeginAlertMontage, 1.0f);

	AnimInstance->Montage_SetEndDelegate(EndDelegate, MonsterMontages->BeginAlertMontage);
	bIsAlerting = true;
	UE_LOG(LogTemp, Log, TEXT("CallAlertDestinationBegin"));
}

void APHScoutMonster::CallAlertDestinationEnd(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Log, TEXT("CallAlertTargetEnd"));
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	SensingComponent->SetActive(false);
}

void APHScoutMonster::SetIsAlerting(bool InIsAlerting)
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


