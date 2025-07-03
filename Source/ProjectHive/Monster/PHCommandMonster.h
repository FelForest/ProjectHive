// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/PHCombatMonsterBase.h"
#include "Interface/PHCommandMonsterInterface.h"
#include "PHCommandMonster.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API APHCommandMonster : public APHCombatMonsterBase,
	public IPHCommandMonsterInterface
{
	GENERATED_BODY()

public:
	APHCommandMonster();

	virtual void CallAlertTargetBegin(APawn* NewTarget) override;
	
	virtual void PropagateTarget(class APawn* NewTarget) override;

	virtual void SetIsOnAlert(bool InIsOnAlert) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetNewDestination(FVector NewDestination) override;

	virtual void OnDead() override;

	virtual void SetTarget(APawn* NewTarget) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Subordinate)
	TArray<APHMonsterBase*> Subordinates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Subordinate)
	int32 OrdinateNum;

	UPROPERTY()
	TScriptInterface<class IPHCommandAIInterface> CommandAI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Investigate)
	uint8 bInvestigating : 1;
};
