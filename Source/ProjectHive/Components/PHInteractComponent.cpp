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
	// ������ �ȰŶ�� ��������Ʈ ����Ұ���
	
	// �ϴ� ��θ� �Ѱɷ� �ϰ� ���� �̱۷�
	IPHInteractableInterface* Interactable = Cast<IPHInteractableInterface>(OtherActor);

	// ���� �ܿ��� Ȯ���� ������ Ȥ�ó� �ؼ� Ȯ�� -> ��κ� �޸� ����
	if (Interactable == nullptr)
	{
		return;
	}
	
	// �̰� �������� ����
	CanInteract = true;

	// TODO : �������� ������ �Ǹ� �迭�� ã�ƾ���

	// Target�� �ֱ�
	Target = OtherActor;

	// UI���� �˸��� ���� ��������Ʈ
	// ����� ���� ���� ������� UI ����ִ� ������� �ϴ���
	// ��Ʈ�ѷ��� ���ε� �Ұ��� -> ĳ���Ϳ��� ������
	// �������, �Ÿ��� �ϴ� ���

	// ó�� �ΰ��� ���� ���Ͷ� ���� �������� Ȯ���ؼ� �����Ÿ� ��������Ʈ ����
	OnInteractTargetOn.ExecuteIfBound(Target);
	

	UE_LOG(LogTemp, Log, TEXT("InteractActorBeginOverLap"));
}
void UPHInteractComponent::OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Ŭ��
	// UI���� ��������Ʈ�� �˷��ֱ�
	// TODO : �迭�̾��ٰ� �Ǵ��� �Ǹ� �׶� nullptr �Ѱ��ִ� ������� ����
	// ������ �׽�Ʈ������ �ϳ��� �ϴ� ���
	OnInteractTargetOff.ExecuteIfBound(Target);
	Target = nullptr;
	
	// ����
	// ��ȣ�ۿ� �׼� ���� ���� false�� ����
	CanInteract = false;

	// ��ȣ�ۿ� ���ϴ� ���� �Ǿ �迭���� ����
	// Remove�� �迭�� ��Ҹ� ������ ���
	// ��� �̵����� ���� ���ذ� ������ �����ߴµ� � ����, �����Ϳ��� ��� ���µ�

	// �̰� ���� Target ���� Ȯ���ؼ� ������ �����

	UE_LOG(LogTemp, Log, TEXT("InteractActorEndOverLap"));
}

