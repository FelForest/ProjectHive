// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PHGrenade.generated.h"

/**
 * Author		: �ӵ���
 * Date			: 2025-06-09
 * Description	:
 * ����ź ����
 * ������ �������� ������ ��������� �ʴ� ����� ������ ���� �ʿ䰡 ���ٰ� �Ǵ�
 * CONSIDER : ȹ��� ����ź, ������ �뵵 ����ź ���� ����°� �´µ�
 * �ݸ����� �ε����� ���� �� ���� ������ �߻�
 * ApplyRadialDamage()�� ����Ͽ� �� ���� ���� ���� ó��
 * ���߽� ����Ʈ + ���� + ī�޶� ��鸲 �߻� ����
 */

class UProjectileMovementComponent;

UCLASS()
class PROJECTHIVE_API APHGrenade : public AActor
{
	GENERATED_BODY()

public:
	APHGrenade();

	// �ܺο��� ������ ���� Getter
	UProjectileMovementComponent* GetProjectileMovement();

	// ���� �Լ�

	// ������ Setter
	FORCEINLINE void SetExplosionDamage(float InDamage)
	{
		// CONSIDER : ������ ���� ��� -> �̰� ȸ�� ����ź ������ ����غ��� �ְڴ�
		ExplosionDamage = FMath::Max(0.0f, InDamage);
	};

	// �ݰ� Setter
	FORCEINLINE void SetExplosionRadius(float InRadius)
	{
		// ���� ����
		ExplosionRadius = FMath::Max(0.0f, InRadius);
	}

	// ������ ���ε� �ϱ� ���ؼ� �������� ����Ǵ� �������� �ϱ� ���� �Լ�
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnGrenadeHit(UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	// ���� ����Ʈ ������ ����, ������ �ϳ� ���� ������ �ִ� �Լ�
	void Explode();

	// ��Ȯ�ϰ� �����Ǵ��� Ȯ���ϱ� ���� �Լ�
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	// �浹 ó�� �ؾ��ϴ� �ݸ��� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
	TObjectPtr<class USphereComponent> CollisionComponent;

	// �����־�� �ϴ� �Ž� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

	// ����ü�̱� ������ ����ü �����Ʈ ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	// ������Ʈ���� �������� ������

	// ������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion")
	float ExplosionDamage;

	// �ݰ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion")
	float ExplosionRadius;

	
};
