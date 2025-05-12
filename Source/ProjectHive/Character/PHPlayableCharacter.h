// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PHPartsCharacter.h"
#include "InputActionValue.h"

#include "Interface/PHAttackableInterface.h"

#include "PHPlayableCharacter.generated.h"

// TODO : Consider Delegate Param
DECLARE_MULTICAST_DELEGATE(FOnEquipmentAcquired);

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API APHPlayableCharacter : 
	public APHPartsCharacter,
	public IPHAttackableInterface
{
	GENERATED_BODY()
	
public:
	APHPlayableCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Delegate Section
	// TODO : Complete
	void SetEquipment(/*InEquipment*/);

protected:
	void Move(const FInputActionValue& Value);

public:
	FOnEquipmentAcquired OnEquipmentAcquired;

protected:
	virtual void BeginPlay() override;

protected:
	// Weapon Section
	// TODO : Request Naming Update
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class UPHWeaponComponent> Weapon;

	// Mesh Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalMesh)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	// Input Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	// Camera Section
	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UCameraComponent> Camera;

};
