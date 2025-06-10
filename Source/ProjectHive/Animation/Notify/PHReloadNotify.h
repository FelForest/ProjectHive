// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PHReloadNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API UPHReloadNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	// 파라미터가 2개있는거랑 3개있는거 찾아보니까 3개짜리가 신버전이라고 함
	// 캐릭터 -> 무기 컴포넌트 -> 무기(총) -> 재장전 함수 호출용 노티파이
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
