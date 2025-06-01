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
 * Author		: 임동현
 * Date			: 2025-05-06
 * Description	:
 * 클라이언트
 *	입력 
 *	애니메이션 재생
 *	
 * 서버
 * 공격 판정
 * 아이템 획득
 * 
 */
UCLASS()
class PROJECTHIVE_API APHPlayableCharacter : public APHPartsCharacter,
	public IPHAttackInterface,
	public IPHItemInterface
{
	GENERATED_BODY()
	
public:
	APHPlayableCharacter();

	// Called when possessed by a player controller to bind input functionality. 
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// AttackInterface
	virtual void Attack() override;

	// ItemInterface
	// Called when the character overlaps with a PHItem
	virtual void PickupItem(class APHItem* InItem) override;

	// Called after all components have been initialized
	virtual void PostInitializeComponents() override;

	// 컨트롤러가 빙의할 때 호출하는 함수
	virtual void PossessedBy(AController* NewController) override;

	// 현재는 public에 있는데 다른곳으로 이동 시킬듯
	// 아이템 오버랩시 호출함 함수
	// UI 띄워주기, 캐릭터 상호작용 활성화 시키기 -> 이거 맞나?
	

protected:
	virtual void BeginPlay() override;

	// Action Section
	void BindInputAction(UEnhancedInputComponent* InEnhancedInputComponent);

	void Move(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);

	void DropWeapon(const FInputActionValue& Value);

	void Attack(const FInputActionValue& Value);

	void SetMappingContext();

	



public:
	FOnEquipmentAcquired OnEquipmentAcquired;

protected:
	// Weapon Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class UPHWeaponComponent> WeaponComponent;

	// Equipment Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	TObjectPtr<class UPHEquipmentComponent> EquipmentComponent;

	// Stat Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	TObjectPtr<class UPHCharacterStatComponent> StatComponent;

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

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = Interact, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class USphereComponent> InteractTrigger;

protected:
	// Stores binding functions matched to input action enum
	TMap<ECharacterActionType, void (APHPlayableCharacter::*)(const FInputActionValue&)> ActionMapping;

	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> InteractAction;

	
};
