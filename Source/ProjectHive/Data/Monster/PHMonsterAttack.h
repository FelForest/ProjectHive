// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PHMonsterAttack.generated.h"

// 몬스터 어떠한 공격이 있는지 파악 하는 용도
UENUM(BlueprintType)
enum class EMonsterAttack : uint8
{
	None = 0,
	Bite = 1,
	Shoot = 2,

	End
};