// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PHCharacterSkeletalMeshData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API UPHCharacterSkeletalMeshData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterMesh)
	TObjectPtr<class USkeletalMesh> HeadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterMesh)
	TObjectPtr<class USkeletalMesh> BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterMesh)
	TObjectPtr<class USkeletalMesh> ArmMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterMesh)
	TObjectPtr<class USkeletalMesh> LegMesh;
};
