// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PHMonsterBase.generated.h"

/**
 * Author		: �ӵ���
 * Date			: 2025-06-11
 * Description	:
 * ������ �⺻�� �Ǵ� Ŭ����
 * ����
 * ���� �� �ֺ��� ����
 * ����
 * �̵�
 * ��е� �ý���
 * ��� ������ ������ ���� ���ȿ� �ΰ� ȿ�� ����
 * ����
 * 
 * ���� ������Ʈ
 * ���� ������Ʈ
 * ���� ������Ʈ
 *
 * ��� ����
 * Attackable Interface�� ������ �ϳ�
 */

UCLASS()
class PROJECTHIVE_API APHMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APHMonsterBase();

protected:

public:	

protected:
	// ����� Ÿ���� �ϳ����� �̷��� ���� Ÿ���� �������� ����� ������� �ϵ��� �ƴϸ� ���赵�� ���� Ÿ���� ���ϰ� �ϴ� ������� �ҵ�
	TObjectPtr<AActor> Target;

	// �׾����� �ƴ��� Ȯ�ο� ����
	UPROPERTY()
	uint8 bisDead : 1;

};
