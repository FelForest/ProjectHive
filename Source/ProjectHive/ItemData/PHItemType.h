// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PHItemType.generated.h"

// TODO : 지금은 타입이 하나만 사용하고 있음
// 본이 다른 타입에서도 쓰는 경우를 고려해야함
UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0,
	Equipment,
	Consumeable,

	End
};