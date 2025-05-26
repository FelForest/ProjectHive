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
	// 무기든 장비든 상관 없이 그냥 UI는 이미지랑 스탯 업데이트 하면 되는거 아닌가..
	void SetEquipment(class APHEquipment* InEquipment);

	// AttackInterface
	virtual void Attack() override;

	// ItemInterface
	// Called when the character overlaps with a PHItem
	virtual void PickupItem(class APHItem* InItem) override;

	// Called after all components have been initialized
	virtual void PostInitializeComponents() override;

	// 현재는 public에 있는데 다른곳으로 이동 시킬듯
	// 아이템 오버랩시 호출함 함수
	// UI 띄워주기, 캐릭터 상호작용 활성화 시키기 -> 이거 맞나?
	UFUNCTION()
	void OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// 오버랩이 끝났을때 호출할 함수
	UFUNCTION()
	void OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

	// Action Section
	void BindInputAction(UEnhancedInputComponent* InEnhancedInputComponent);

	void Move(const FInputActionValue& Value);

	void Interact(AActor* InInteractor);

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

protected:
	// Stores binding functions matched to input action enum
	TMap<ECharacterActionType, void (APHPlayableCharacter::*)(const FInputActionValue&)> ActionMapping;
};
