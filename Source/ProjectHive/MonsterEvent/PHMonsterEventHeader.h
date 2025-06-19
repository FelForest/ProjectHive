// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PHMonsterEventType.h"
#include "PHMonsterEventStage.h"
#include "PHMonsterEventPayload.h"
#include "PHMonsterEventHeader.generated.h"

// TODO : 지금은 타입이 하나만 사용하고 있음
// 본이 다른 타입에서도 쓰는 경우를 고려해야함
USTRUCT()
struct FMonsterEventHeader
{
	GENERATED_BODY()

	UPROPERTY()
	EMonsterEventType Type;

	UPROPERTY()
	TWeakObjectPtr<class APHMonsterBase> Sender;

	UPROPERTY()
	EMonsterEventStage Stage;

	UPROPERTY()
	TObjectPtr<UPHMonsterEventPayload> Payload;

};