// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PHPartsCharacter.h"
#include "InputActionValue.h"
#include "PHAttackableCharacter.generated.h"

// TODO : Consider Delegate Param
DECLARE_MULTICAST_DELEGATE(FOnEquipmentAcquired);

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API APHAttackableCharacter : public APHPartsCharacter
{
	GENERATED_BODY()
	
public:
	APHAttackableCharacter();

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
};
