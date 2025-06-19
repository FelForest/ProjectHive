// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PHMonsterEventType.generated.h"

UENUM(BlueprintType)
enum class EMonsterEventType : uint8
{
	None,
	TargetDetected,
	RequestAssist,
	CommanderDeath,
	// ...
};