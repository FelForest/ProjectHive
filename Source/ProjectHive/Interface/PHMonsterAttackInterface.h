// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PHMonsterAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPHMonsterAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Author		: 임동현
 * Date			: 2025-06-15
 * Description	:
 * 공격이 가능한 몬스터 인터페이스
 */
class PROJECTHIVE_API IPHMonsterAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Attack() = 0;
};
