// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PHMonsterStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTHIVE_API UPHMonsterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPHMonsterStatComponent();

	UFUNCTION()
	FORCEINLINE float GetAlertRadius() const
	{
		return AlertRadius;
	}


protected:
	
	
public:	
	

public:
	



protected:
	// 전파 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat);
	float AlertRadius;
};
