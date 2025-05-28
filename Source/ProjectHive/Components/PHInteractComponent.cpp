// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PHInteractComponent.h"
#include "Interface/PHInteractableInterface.h"

// Sets default values for this component's properties
UPHInteractComponent::UPHInteractComponent()
{

}

void UPHInteractComponent::Interact()
{
	if (!CanInteract)
	{
		return;
	}
	IPHInteractableInterface* Interactable = Cast<IPHInteractableInterface>(Target);


	if (Interactable == nullptr)
	{
		return;
	}

	Interactable->Interact(GetOwner());
}

void UPHInteractComponent::OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �ϴ� ��θ� �Ѱɷ� �ϰ� ���� �̱۷�
	IPHInteractableInterface* Interactable = Cast<IPHInteractableInterface>(OtherActor);

	// ���� �ܿ��� Ȯ���� ������ Ȥ�ó� �ؼ� Ȯ�� -> ��κ� �޸� ����
	if (Interactable == nullptr)
	{
		return;
	}
	
	if (GetOwner()->HasAuthority())
	{
		CanInteract = true;
	}

	// �迭�� �߰��� ���� UI ���� Ÿ�� ����
	Targets.Add(OtherActor);
	Target = Targets[0];

	// ���� Ÿ���� ���� Ÿ���̶� �ٸ��� UI ���� �� ���� Ÿ�� ����
	// ���⼭ ���� Ÿ���� nullptr�� ��쿡�� ����
	if (PreviousTarget != Target)
	{
		OnInteractTargetOn.ExecuteIfBound(Target);
		PreviousTarget = Target;
	}

	UE_LOG(LogTemp, Log, TEXT("InteractActorBeginOverLap"));
}
void UPHInteractComponent::OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// ����
	// ��ȣ�ۿ� �׼� ���� ���� false�� ����

	// ���� ��� ���Ͱ� ���� UI����ְ� �ִ� ��ü���� Ȯ�� �� UI ����
	if (Target == OtherActor)
	{
		OnInteractTargetOff.ExecuteIfBound(OtherActor);
	}

	// ������ ��ҵ��� ������ ����� ������ �Ǿ� �ִµ� ���� �ΰ����� ������ ���Ƶ� 8���� ����
	Targets.Remove(OtherActor);
	
	// ���� �迭�� ũ�⸦ Ȯ�� ��, ���� Ÿ���� ������ UI ȣ�� �ƴϸ� �ʱ�ȭ
	if (Targets.Num() > 0)
	{
		Target = Targets[0];
		OnInteractTargetOn.ExecuteIfBound(Target);
		PreviousTarget = Target;
	}
	else
	{
		PreviousTarget = nullptr;
		if (GetOwner()->HasAuthority())
		{
			CanInteract = false;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("InteractActorEndOverLap"));
}

