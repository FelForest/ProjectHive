// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PHMonsterSensingComponent.h"

// 이거 상위 생성자는 파라미터가 있는데 이거 받아서 만들어야 하는지는 모르겠군요
UPHMonsterSensingComponent::UPHMonsterSensingComponent()
{
	LOSHearingThreshold = 0.0f;
	HearingThreshold = 0.0f;
	SightRadius = 1000.0f;
	PeripheralVisionAngle = 60.f;
	PeripheralVisionCosine = FMath::Cos(FMath::DegreesToRadians(PeripheralVisionAngle));

	SensingInterval = 0.5f;
	HearingMaxSoundAge = 1.f;

	bOnlySensePlayers = true;
	bHearNoises = false;
	bSeePawns = true;

	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	bAutoActivate = true;

	bEnableSensingUpdates = true;
}


