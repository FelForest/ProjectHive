// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/Monster/PHMonsterAttack.h"
#include "PHMonsterAttackComponentFactory.generated.h"

/**
 * Author		: 임동현
 * Date			: 2025-06-17
 * Description	:
 * MonsterAttackComponent를 생성하는 Factory
 * 몬스터가 필요한 공격 컴포넌트마다 헤더에 추가하는 방식이 불편하다고 판단
 */

class UPHMonsterAttackComponent;

UCLASS()
class PROJECTHIVE_API UPHMonsterAttackComponentFactory : public UObject
{
	GENERATED_BODY()
	
public :
	static UPHMonsterAttackComponent* CreateMonsterAttackComponent(class AActor* Owner, EMonsterAttack InAttackType);
};
