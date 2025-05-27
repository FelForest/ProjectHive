// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PHInteractComponent.generated.h"

// ���� ����
class IPHInteractableInterface;

// ��ȣ�ۿ� ������ ���� ������ �ƴٰ� ����� ��������Ʈ
// �̰� ��Ʈ�ѷ�(UI) ������ ���ε� -> ���� Ȯ���� ���� ���� ����
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

	// �������� �������� ȣ���� �Լ�
	UFUNCTION()
	void OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Delegate Section
	FOnInteractTargetOnDelegate OnInteractTargetOn;

	FOnInteractTargetOffDelegate OnInteractTargetOff;

private:
	// ĳ������ ����� ��ȣ�ۿ� ������ ����
	TObjectPtr<class AActor> Target;

	// �������� UI�� �����ؾ����� ���ƾ� ���� �Ǵ��� �ϱ����� ���� ����
	TObjectPtr<class AActor> PreviousTarget;

	// ������ ������ �� ó���ϱ� ���� ��ȣ�ۿ� ������ ���� �迭
	// Target ���� �� ���� -> �迭 ���� ����°� ����ó����
	TArray<TObjectPtr<class AActor>> Targets;

	bool CanInteract = false;
};