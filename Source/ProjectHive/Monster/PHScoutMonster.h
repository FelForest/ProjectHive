// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/PHMonsterBase.h"
#include "PHScoutMonster.generated.h"

/**
 * Author		: �ӵ���
 * Date			: 2025-06-20
 * Description	:
 * ������ ������ �˸��⸸ �ϴ� ����
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

	// �̰͵� �������̽��� �������ϴ°ǰ�
	void SetDestination(FVector NewDestination);


	// AI ĳ���� �ϱ� ���� ����
	virtual void PossessedBy(AController* NewController) override;

private:
	UPROPERTY()
	TScriptInterface<class IPHScoutAIInterface> ScoutAI;

	UPROPERTY()
	FVector Destination;
};
