// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PHCharacterBase.h"
#include "PHPartsCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHIVE_API APHPartsCharacter : public APHCharacterBase
{
	GENERATED_BODY()
	
public:
	APHPartsCharacter();


protected:
	virtual void BeginPlay() override;

	void SetCharacterPartsMesh();

protected:
	// SketalMesh Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalMesh)
	TObjectPtr<USkeletalMeshComponent> HeadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalMesh)
	TObjectPtr<USkeletalMeshComponent> ArmMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalMesh)
	TObjectPtr<USkeletalMeshComponent> LegMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalMeshData)
	TObjectPtr<class UPHCharacterSkeletalMeshData> PartsMeshData;

};
