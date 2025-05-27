// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PHPartsCharacter.h"

#include "InputActionValue.h"
#include "Input/PHCharacterInputActionData.h"

#include "Interface/PHAttackInterface.h"
#include "Interface/PHItemInterface.h"


#include "PHPlayableCharacter.generated.h"

// TODO: Define parameters for equipment delegate
DECLARE_MULTICAST_DELEGATE(FOnEquipmentAcquired);

/**
 * Author		: �ӵ���
 * Date			: 2025-05-06
 * Description	:
 * Ŭ���̾�Ʈ
 *	�Է� 
 *	�ִϸ��̼� ���
 *	
 * ����
 * ���� ����
 * ������ ȹ��
 * 
 */
UCLASS()
class PROJECTHIVE_API APHPlayableCharacter : 
	public APHPartsCharacter,
	public IPHAttackInterface,
	public IPHItemInterface
{
	GENERATED_BODY()
	
public:
	APHPlayableCharacter();

	// Called when possessed by a player controller to bind input functionality. 
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Delegate Section
	// ����� ���� ��� ���� �׳� UI�� �̹����� ���� ������Ʈ �ϸ� �Ǵ°� �ƴѰ�..
	void SetEquipment(class APHEquipment* InEquipment);

	// AttackInterface
	virtual void Attack() override;

	// ItemInterface
	// Called when the character overlaps with a PHItem
	virtual void PickupItem(class APHItem* InItem) override;

	// Called after all components have been initialized
	virtual void PostInitializeComponents() override;

	// ����� public�� �ִµ� �ٸ������� �̵� ��ų��
	// ������ �������� ȣ���� �Լ�
	// UI ����ֱ�, ĳ���� ��ȣ�ۿ� Ȱ��ȭ ��Ű�� -> �̰� �³�?
	

protected:
	virtual void BeginPlay() override;

	// Action Section
	void BindInputAction(UEnhancedInputComponent* InEnhancedInputComponent);

	void Move(const FInputActionValue& Value);

	void Interact();

public:
	FOnEquipmentAcquired OnEquipmentAcquired;

protected:
	// Weapon Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class UPHWeaponComponent> WeaponComponent;

	// Input Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPHCharacterInputActionData> CharacterInputActionData;

	// Camera Section
	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category = Interact, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UPHInteractComponent> InteractComponent;

protected:
	// Stores binding functions matched to input action enum
	//TMap<ECharacterActionType, TFunction<void(const FInputActionValue&)>> ActionMapping;

	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> InteractAction;
};
