// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PHPartsCharacter.h"

#include "InputActionValue.h"
#include "Input/PHCharacterInputActionData.h"

#include "Interface/PHAttackInterface.h"


#include "PHPlayableCharacter.generated.h"

// TODO: Define parameters for equipment delegate
DECLARE_MULTICAST_DELEGATE(FOnEquipmentAcquired);

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API APHPlayableCharacter : 
	public APHPartsCharacter,
	public IPHAttackInterface
{
	GENERATED_BODY()
	
public:
	APHPlayableCharacter();

	// Called when possessed by a player controller to bind input functionality. 
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Delegate Section
	// TODO : Complete
	void SetEquipment(/*InEquipment*/);

	virtual void Attack() override;

protected:
	virtual void BeginPlay() override;

	void BindInputAction(UEnhancedInputComponent* InEnhancedInputComponent);

	void Move(const FInputActionValue& Value);

public:
	FOnEquipmentAcquired OnEquipmentAcquired;

protected:
	// Weapon Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class UPHWeaponComponent> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalMesh)
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;

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
