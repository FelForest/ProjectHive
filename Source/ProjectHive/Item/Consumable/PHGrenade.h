// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PHGrenade.generated.h"

/**
 * Author		: 임동현
 * Date			: 2025-06-09
 * Description	:
 * 수류탄 액터
 * 원래는 아이템이 맞지만 사용하지도 않는 기능을 가지고 있을 필요가 없다고 판단
 * CONSIDER : 획득용 수류탄, 던지는 용도 수류탄 따로 만드는게 맞는듯
 * 콜리전에 부딪히면 폭발 및 범위 데미지 발생
 * ApplyRadialDamage()를 사용하여 구 형태 범위 판정 처리
 * 폭발시 이펙트 + 사운드 + 카메라 흔들림 발생 예정
 */

class UProjectileMovementComponent;

UCLASS()
class PROJECTHIVE_API APHGrenade : public AActor
{
	GENERATED_BODY()

public:
	APHGrenade();

	// 외부에서 세팅을 위한 Getter
	UProjectileMovementComponent* GetProjectileMovement();

	// 폭발 함수

	// 데미지 Setter
	FORCEINLINE void SetExplosionDamage(float InDamage)
	{
		// CONSIDER : 음수값 방지 방안 -> 이거 회복 수류탄 같은거 고민해볼수 있겠다
		ExplosionDamage = FMath::Max(0.0f, InDamage);
	};

	// 반경 Setter
	FORCEINLINE void SetExplosionRadius(float InRadius)
	{
		// 음수 방지
		ExplosionRadius = FMath::Max(0.0f, InRadius);
	}

	// 오버랩 바인딩 하기 위해서 안전성이 보장되는 시점에서 하기 위한 함수
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnGrenadeHit(UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	// 폭발 이펙트 생성및 사운드, 오버랩 하나 만들어서 데미지 주는 함수
	void Explode();

	// 정확하게 삭제되는지 확인하기 위한 함수
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	class USphereComponent* GetCollisionComponent();

protected:
	// 충돌 처리 해야하는 콜리전 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
	TObjectPtr<class USphereComponent> CollisionComponent;

	// 보여주어야 하는 매시 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

	// 투사체이기 때문에 투사체 무브먼트 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	// 폭발 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	TObjectPtr<class UNiagaraSystem> ExplosionEffect;

	// 폭발 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	TObjectPtr<class USoundBase> ExplosionSound;

	// 컴포넌트에서 설정해줄 데이터

	// 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion")
	float ExplosionDamage;

	// 반경
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion")
	float ExplosionRadius;

	UPROPERTY()
	FTimerHandle ExplodeTimerHandle;

	UPROPERTY()
	float DelayTime;
	
};
