// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PHMonsterAlertNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API UPHMonsterAlertNotify : public UAnimNotify
{
	GENERATED_BODY()

	// 경고 노티파이
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
