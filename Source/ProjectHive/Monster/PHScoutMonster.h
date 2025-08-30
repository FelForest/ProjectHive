// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/PHMonsterBase.h"
#include "Interface/PHScoutInterface.h"
#include "PHScoutMonster.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API APHScoutMonster : public APHMonsterBase,
	public IPHScoutInterface
{
	GENERATED_BODY()
	
public:
	APHScoutMonster();

	virtual void ReportTargetLocationToCommander(FVector NewDestination) override;
};
