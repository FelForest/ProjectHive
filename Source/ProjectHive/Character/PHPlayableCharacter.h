// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PHPartsCharacter.h"

#include "InputActionValue.h"
#include "Input/PHCharacterInputActionData.h"

#include "Interface/PHAttackInterface.h"
#include "Interface/PHItemInterface.h"
#include "Interface/PHGrenadeThrowInterface.h"
#include "Interface/PHWeaponAnimOwnerInterface.h"
#include "Interface/PHWeaponChangeInterface.h"

#include "Components/TimelineComponent.h"

#include "PHPlayableCharacter.generated.h"





/**
 * Author		: 임동현
 * Date			: 2025-05-06
 * Description	:
 * 클라이언트
 *	입력 
 *	애니메이션 재생
 *	
 * 서버
 * 공격 판정
 * 아이템 획득
 * 
 */

class UPHWeaponComponent;

 // TODO: Define parameters for equipment delegate
DECLARE_MULTICAST_DELEGATE(FOnEquipmentAcquired);

UCLASS()
class PROJECTHIVE_API APHPlayableCharacter : public APHPartsCharacter,
	public IPHAttackInterface,
	public IPHItemInterface,
	public IPHGrenadeThrowInterface,
	public IPHWeaponAnimOwnerInterface,
	public IPHWeaponChangeInterface
{
	GENERATED_BODY()
	
public:
	APHPlayableCharacter();

	// Called when possessed by a player controller to bind input functionality. 
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// AttackInterface
	virtual void Attack() override;

	// ItemInterface
	// Called when the character overlaps with a PHItem
	virtual void PickupItem(class APHItem* InItem) override;

	// Called after all components have been initialized
	virtual void PostInitializeComponents() override;

	// 컨트롤러가 빙의할 때 호출하는 함수
	virtual void PossessedBy(AController* NewController) override;

	// 무기가 장착 되었을때 애니메이션 세팅하는 함수
	virtual void OnWeaponEquipped() override;

	// 무기가 해제 되었을때 이니메이션 세팅하는 함수
	virtual void OnWeaponUnequipped() override;

	virtual void ChangeWeapon() override;
	

	// 현재는 public에 있는데 다른곳으로 이동 시킬듯
	// 아이템 오버랩시 호출함 함수
	// UI 띄워주기, 캐릭터 상호작용 활성화 시키기 -> 이거 맞나?
	
	// 컴포넌트 들고오기
	

	// 애님 인스턴스에 사용할 함수
	// 현재 캐릭터가 어느 방향으로 가는지 알기 위한 함수

	// 현재 조준중인지 알기 위함 함수
	UFUNCTION()
	bool IsAiming() const;

	UFUNCTION()
	UPHWeaponComponent* GetWeaponComponent() const;

	UFUNCTION()
	float GetAimDirection() const;

	UFUNCTION()
	float GetTensionLevel() const;

	virtual void ThrowGrenade() override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// ConstructorHelpers 너무 길어서 따로 뺌
	void LoadAssets();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// 타임라인 초기 설정에 필요한게 많아서 따로 뺌
	void InitializeTimeLine();

	// 액션 매핑하는게 너무 커져서 따로 빼버림
	void InitializeActionMappings();

	// Action Section
	void BindInputAction(UEnhancedInputComponent* InEnhancedInputComponent);

	void Move(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);

	void DropWeapon(const FInputActionValue& Value);

	void Attack(const FInputActionValue& Value);

	void AimStart(const FInputActionValue& Value);

	void AimHold(const FInputActionValue& Value);

	void AimEnd(const FInputActionValue& Value);

	void SwapWeapon(const FInputActionValue& Value);

	void RunStart(const FInputActionValue& Value);

	void RunEnd(const FInputActionValue& Value);

	void Reload(const FInputActionValue& Value);

	void ThrowGrenade(const FInputActionValue& Value);

	void SetMappingContext();
	
	// 입력시 마우스 위치 업데이트
	bool UpdateMouseLocation();

	// 상태에 따른 이동 속도 변경 함수
	void SetMovementSpeed();

	// 경계 레벨을 감소 시키는 함수
	UFUNCTION()
	void DecreaseTensionLevel(float Value);

	// 경계 레벨을 증가 시키는 함수
	UFUNCTION()
	void IncreaseTensionLevel(float Value);

	// 공격 몽타주가 끝나는것을 알기 위한 함수
	UFUNCTION()
	void OnAttackActionMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void StartIncreaseTinesionFromCurrentLevel();

	UFUNCTION()
	void SetActorToTargetRotation(float Value);

	// PostAnimEvaluation() 재귀 호출로 인한 크러시를 방지를 위한 함수
	UFUNCTION()
	void OnDeferredWeaponEquipped();

public:
	FOnEquipmentAcquired OnEquipmentAcquired;

protected:
	// Weapon Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<UPHWeaponComponent> WeaponComponent;

	// Equipment Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	TObjectPtr<class UPHEquipmentComponent> EquipmentComponent;

	// Stat Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	TObjectPtr<class UPHCharacterStatComponent> StatComponent;

	// Grenade Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grenade)
	TObjectPtr<class UPHGrenadeComponent> GrenadeComponent;

	// Input Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPHCharacterInputActionData> CharacterInputActionData;

	// Camera Section
	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UCameraComponent> Camera;

	// Interact Section
	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category = Interact, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UPHInteractComponent> InteractComponent;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = Interact, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class USphereComponent> InteractTrigger;


	
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UAnimMontage> AttackActionMontage;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UAnimMontage> ChangeWeaponMontage;

	UPROPERTY(EditAnywhere, Category = Anim)
	TSubclassOf<class UAnimInstance> CharacterAnim;

protected:
	// Stores binding functions matched to input action enum
	TMap<ECharacterActionType, void (APHPlayableCharacter::*)(const FInputActionValue&)> ActionMapping;

	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;

	UPROPERTY()
	TObjectPtr<class APHPlayerController> PlayerController;

	UPROPERTY()
	FVector MouseLocation;

	// MoveSpeed Section
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = MoveSpeed, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = MoveSpeed, meta = (AllowPrivateAccess = "true"))
	float AimSpeed;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = MoveSpeed, meta = (AllowPrivateAccess = "true"))
	float RunSpeed;

	UPROPERTY()
	uint8 bIsRunning : 1;

	// Aim Section
	UPROPERTY()
	uint8 bIsAiming : 1;

	// 조준 방향과 이동방향의 각도
	UPROPERTY()
	float AimDirection;

	// 애니메이션의 상태를 결정하는 경계 레벨
	UPROPERTY()
	float TensionLevel;

	// 조준 해제 후 경계 레벨을 감소시키는 타이머 핸들
	FTimerHandle TensionLevelDecayHandle;

	// 경계 레벨 감소 비율
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = TensionLevel, meta = (AllowPrivateAccess = "true"))
	float TensionDecayRate;

	// 경계레벨 최대치
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = TensionLevel, meta = (AllowPrivateAccess = "true"))
	float MaxTensionLevel;

	// 경계레벨 최소치
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = TensionLevel, meta = (AllowPrivateAccess = "true"))
	float MinTensionLevel;

	// 경계레벨을 보간하기 위한 타임라인 선언
	FTimeline TensionIncreaseTimeline;
	FTimeline TensionDecreaseTimeline;

	// 사용할 커브
	UPROPERTY(EditDefaultsOnly, Category = TensionLevel)
	UCurveFloat* TensionIncreaseCurve;
	
	UPROPERTY(EditDefaultsOnly, Category = TensionLevel)
	UCurveFloat* TensionDecreaseCurve;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = FireRate, meta = (AllowPrivateAccess = "ture"))
	float FireRate;

	UPROPERTY()
	uint8 bIsAttacking : 1;


	// 캐릭터 회전에 따른 보간 처리를 위한 타임라인
	UPROPERTY()
	FTimeline SettingRotationTimeline;

	// 사용할 커브
	UPROPERTY()
	UCurveFloat* RotationCurve;

	UPROPERTY()
	int32 SwapDirection;

};
