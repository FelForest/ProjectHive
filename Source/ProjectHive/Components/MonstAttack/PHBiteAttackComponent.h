// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PHMonsterAttackComponent.h"
#include "PHBiteAttackComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API UPHBiteAttackComponent : public UPHMonsterAttackComponent
{
	GENERATED_BODY()
	
public:
	UPHBiteAttackComponent();

	virtual void Attack() override;

};
