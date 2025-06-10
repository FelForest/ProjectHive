// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PHWeaponAnimOwnerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPHWeaponAnimOwnerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTHIVE_API IPHWeaponAnimOwnerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// ���� ���� �� �ִϸ��̼� ���� ����
	UFUNCTION()
	virtual void OnWeaponEquipped() = 0;

	// ���� ���� �� �ִϸ��̼� �ʱ�ȭ
	UFUNCTION()
	virtual void OnWeaponUnequipped() = 0;
};
