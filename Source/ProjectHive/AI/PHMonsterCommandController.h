// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/PHMonsterController.h"
#include "Interface/PHCommandAIInterface.h"
#include "PHMonsterCommandController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API APHMonsterCommandController : public APHMonsterController,
	public IPHCommandAIInterface
{
	GENERATED_BODY()
	
public:
	APHMonsterCommandController();

	virtual void SetIsOnAlert(bool InIsOnAlert) override;
};
