// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PHMonsterEventStage.generated.h"

UENUM(BlueprintType)
enum class EMonsterEventStage : uint8
{
	Init,
	PreProgress,
	OnProgress,
	PostProgress
};