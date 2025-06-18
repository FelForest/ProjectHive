// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PHMonsterStatComponent.h"

// Sets default values for this component's properties
UPHMonsterStatComponent::UPHMonsterStatComponent()
{
	// TODO : 데이터 외부에터 받아오기
	AlertRadius = 5.0f;

	Hp = 10.0f;

	MoveSpeed = 600.0f;

	AttackRange = 500.0f;
}

void UPHMonsterStatComponent::ChangeHP(float InDamage)
{
	// 마음 같아서는 공통으로 있는거 상위로 뽑아서 캐릭터용 몬스터용 만들고 싶지만 패스
	// 내부에서 방어력이나 이런거 다 해줘야 하는데 지금은 없으니까 패스
	if (Hp <= 0.0f)
	{
		return;
	}

	Hp -= InDamage;

	UE_LOG(LogTemp, Log, TEXT("%s HP : %f"), *GetOwner()->GetName(), Hp);

	// 죽었을때
	if (Hp <= KINDA_SMALL_NUMBER)
	{
		Hp = 0.0f;
		OnDead.Broadcast();
	}

	OnHPChange.Broadcast(Hp);
}
