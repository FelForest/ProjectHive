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
	// 일단 고민만 한걸로 하고 먼저 싱글로
	IPHInteractableInterface* Interactable = Cast<IPHInteractableInterface>(OtherActor);

	// 엔진 단에서 확인은 하지만 혹시나 해서 확인 -> 대부분 휴먼 오류
	if (Interactable == nullptr)
	{
		return;
	}
	
	if (GetOwner()->HasAuthority())
	{
		CanInteract = true;
	}

	// 배열에 추가후 현재 UI 띄우는 타겟 설정
	Targets.Add(OtherActor);
	Target = Targets[0];

	// 이전 타겟이 현재 타겟이랑 다르면 UI 생성 및 이전 타겟 갱신
	// 여기서 이전 타겟이 nullptr인 경우에도 가능
	if (PreviousTarget != Target)
	{
		OnInteractTargetOn.ExecuteIfBound(Target);
		PreviousTarget = Target;
	}
}
void UPHInteractComponent::OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 서버
	// 상호작용 액션 가능 값을 false로 변경

	// 범위 벗어난 액터가 현재 UI띄워주고 있는 객체인지 확인 후 UI 갱신
	if (Target == OtherActor)
	{
		OnInteractTargetOff.ExecuteIfBound(OtherActor);
	}

	// 삭제시 요소들이 앞으로 당겨져 오도록 되어 있는데 실제 인게임의 갯수는 많아도 8개가 끝임
	Targets.Remove(OtherActor);
	
	// 현재 배열의 크기를 확인 후, 다음 타겟이 있으면 UI 호출 아니면 초기화
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
}

