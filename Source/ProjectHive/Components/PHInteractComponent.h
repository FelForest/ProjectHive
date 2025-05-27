// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PHInteractComponent.generated.h"

// 전방 선언
class IPHInteractableInterface;

// 상호작용 가능한 액터 오버랩 됐다고 사용할 델리게이트
// 이건 컨트롤러(UI) 에서만 바인딩 -> 추후 확장을 위해 변경 가능
DECLARE_DELEGATE_OneParam(FOnInteractTargetOnDelegate, AActor* /*NewTarget*/);
DECLARE_DELEGATE_OneParam(FOnInteractTargetOffDelegate, AActor* /*NewTarget*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTHIVE_API UPHInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPHInteractComponent();

	void Interact();

	UFUNCTION()
	void OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// 오버랩이 끝났을때 호출할 함수
	UFUNCTION()
	void OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Delegate Section
	FOnInteractTargetOnDelegate OnInteractTargetOn;

	FOnInteractTargetOffDelegate OnInteractTargetOff;

private:
	// 캐스팅후 사용할 상호작용 가능한 액터
	TObjectPtr<class AActor> Target;

	// 오버랩시 UI를 변경해야할지 말아야 할지 판단을 하기위한 이전 액터
	TObjectPtr<class AActor> PreviousTarget;

	// 여러개 들어왔을 때 처리하기 위한 상호작용 가능한 액터 배열
	// Target 따로 쓴 이유 -> 배열 범위 벗어나는거 예외처리용
	TArray<TObjectPtr<class AActor>> Targets;

	bool CanInteract = false;
};