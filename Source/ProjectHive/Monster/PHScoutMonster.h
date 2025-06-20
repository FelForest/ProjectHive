// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/PHMonsterBase.h"
#include "PHScoutMonster.generated.h"

/**
 * Author		: 임동현
 * Date			: 2025-06-20
 * Description	:
 * 감지시 상위에 알리기만 하는 몬스터
 * 
 */
UCLASS()
class PROJECTHIVE_API APHScoutMonster : public APHMonsterBase,
	public IPHAlertCallerInterface
{
	GENERATED_BODY()

public:
	APHScoutMonster();


	// AlertCallerInterface Section

	virtual void CallAlertDestination() override;

	virtual void CallAlertDestinationBegin(APawn* NewTarget) override;

	virtual void CallAlertDestinationEnd(class UAnimMontage* Montage, bool bInterrupted) override;

	virtual void SetIsAlerting(bool InIsAlerting) override;

	virtual void SetMonsterAlertDelegate(const FMonsterAlertFinished& InOnAlertFinished) override;

	// 이것도 인터페이스로 강제해하는건가
	void SetDestination(FVector NewDestination);


	// AI 캐스팅 하기 위한 시점
	virtual void PossessedBy(AController* NewController) override;

private:
	UPROPERTY()
	TScriptInterface<class IPHScoutAIInterface> ScoutAI;

	UPROPERTY()
	FVector Destination;
};
