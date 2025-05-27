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
	// 오버랩 된거라고 델리게이트 방송할거임
	
	// 일단 고민만 한걸로 하고 먼저 싱글로
	IPHInteractableInterface* Interactable = Cast<IPHInteractableInterface>(OtherActor);

	// 엔진 단에서 확인은 하지만 혹시나 해서 확인 -> 대부분 휴먼 오류
	if (Interactable == nullptr)
	{
		return;
	}
	
	// 이건 서버에서 설정
	CanInteract = true;

	// TODO : 오버랩이 여러개 되면 배열로 찾아야함

	// Target에 넣기
	Target = OtherActor;

	// UI에게 알리기 위한 델리게이트
	// 현재는 먼저 들어온 순서대로 UI 띄워주는 방법으로 하는중
	// 컨트롤러가 바인딩 할거임 -> 캐릭터에서 시켜줌
	// 고려사항, 거리로 하는 방법

	// 처음 인경우와 이전 액터랑 같은 액터인지 확인해서 같으거면 델리게이트 막기
	OnInteractTargetOn.ExecuteIfBound(Target);
	

	UE_LOG(LogTemp, Log, TEXT("InteractActorBeginOverLap"));
}
void UPHInteractComponent::OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 클라
	// UI에게 딜리게이트로 알려주기
	// TODO : 배열이없다고 판단이 되면 그때 nullptr 넘겨주는 방식으로 변경
	// 지금은 테스트용으로 하나만 하는 경우
	OnInteractTargetOff.ExecuteIfBound(Target);
	Target = nullptr;
	
	// 서버
	// 상호작용 액션 가능 값을 false로 변경
	CanInteract = false;

	// 상호작용 안하는 것이 되어서 배열에서 제거
	// Remove는 배열의 요소를 앞으로 당김
	// 요소 이동으로 인한 손해가 많은가 생각했는데 몇개 없고, 포인터여서 상관 없는듯

	// 이게 현재 Target 인지 확인해서 삭제해 줘야함

	UE_LOG(LogTemp, Log, TEXT("InteractActorEndOverLap"));
}

