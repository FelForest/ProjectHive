// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHPlayableCharacter.h"

// Camera Section
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


// Movement Section
#include "GameFramework/CharacterMovementComponent.h"

//#include "Input/PHCharacterInputActionData.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Interact Section
#include "Interface/PHInteractableInterface.h"
#include "Components/PHInteractComponent.h"

// Item Including Section
#include "Item/PHItem.h"
#include "Item/Equipment/PHEquipment.h"
#include "Item/Equipment/Weapon/PHWeapon.h"

// Equipment Section
#include "Weapon/PHWeaponComponent.h"
#include "Components/PHEquipmentComponent.h"

// Stat Section
#include "Components/PHCharacterStatComponent.h"

// Grenade Section
#include "Components/PHGrenadeComponent.h"

// OverLap Section
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

// 임시용
#include "Player/PHPlayerController.h"

// UI Section
#include "UI/PHHUDWidget.h"


APHPlayableCharacter::APHPlayableCharacter()
{
	// 캐릭터 프리셋 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Playable"));

	// Tick 활성화
	PrimaryActorTick.bCanEverTick = true;

	// Setting Stat
	StatComponent = CreateDefaultSubobject<UPHCharacterStatComponent>(TEXT("StatComponent"));

	// Setting Weapon 
	WeaponComponent = CreateDefaultSubobject<UPHWeaponComponent>(TEXT("WeaponComponent"));

	// Setting Equipment
	EquipmentComponent = CreateDefaultSubobject<UPHEquipmentComponent>(TEXT("EquipmentComponent"));

	// Setting Grenade
	GrenadeComponent = CreateDefaultSubobject<UPHGrenadeComponent>(TEXT("GrenadeComponent"));

	// LoadAssets
	LoadAssets();

	SetReplicates(true);

	// Setting Camera
	// TODO : Consider moving to an attackable character
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	// TODO : Update later
	// length, angle, etc...
	SpringArm->TargetArmLength = 1400.0f;
	SpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	SpringArm->bDoCollisionTest = false;

	// 캐릭터 회전에 따른 컨트롤러 회전 막기
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// 이게 가능한 이유 : 맴버 변수로 가지고 있어서 이게 먼저 초기화되는게 보장됨
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bIsRunning = false;

	// Setting Aim
	bIsAiming = false;

	// Setting TensionLevel
	TensionDecayRate = 1.0f;
	MaxTensionLevel = 100.0f;
	MinTensionLevel = 0.0f;

	// Setting InteractComponent
	InteractComponent = CreateDefaultSubobject<UPHInteractComponent>(TEXT("InteractComponent"));

	// Setting InteractTrigger
	InteractTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("InteractTrigger"));
	InteractTrigger->SetupAttachment(RootComponent);
	InteractTrigger->SetCollisionProfileName(TEXT("Interaction"));
	// TODO : 매직넘버 데이터로 받아오기 변경
	InteractTrigger->SetSphereRadius(98.5f);

	// Setting Action
	InitializeActionMappings();

	// TODO : 발사 속도 조절을 위해서 어쩌구 저쩌구 Test
	FireRate = 1.0f;
	bIsAttacking = false;

	bIsGrenadeAiming = false;

	// 죽은 상태 평가를 위한 변수 설정
	bIsDead = false;
}

bool APHPlayableCharacter::IsAiming() const
{
	return bIsAiming;
}

UPHWeaponComponent* APHPlayableCharacter::GetWeaponComponent() const
{
	return WeaponComponent;
}

float APHPlayableCharacter::GetAimDirection() const
{
	return AimDirection;
}

float APHPlayableCharacter::GetTensionLevel() const
{
	return TensionLevel;
}

void APHPlayableCharacter::LoadAssets()
{
	// Load AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> CharacterAnimRef(TEXT("/Game/ProjectHive/Animation/ABP_Base.ABP_Base_C"));
	if (CharacterAnimRef.Class)
	{
		CharacterAnim = CharacterAnimRef.Class;
		GetMesh()->SetAnimClass(CharacterAnimRef.Class);
	}

	// Load CharacterInputActionData
	static ConstructorHelpers::FObjectFinder<UPHCharacterInputActionData> CharacterInputActionDataRef = TEXT("/Game/ProjectHive/Input/PHC_InputActionData.PHC_InputActionData");
	if (CharacterInputActionDataRef.Object != nullptr)
	{
		CharacterInputActionData = CharacterInputActionDataRef.Object;
	}

	// Load TensionDecreaseCurve
	static ConstructorHelpers::FObjectFinder<UCurveFloat> TensionDecreaseCurveRef = TEXT("/Game/ProjectHive/Curve/Curve_Tension_Decay.Curve_Tension_Decay");
	if (TensionDecreaseCurveRef.Object != nullptr)
	{
		TensionDecreaseCurve = TensionDecreaseCurveRef.Object;
	}

	// Load TensionIncreaseCurve
	static ConstructorHelpers::FObjectFinder<UCurveFloat> TensionIncreaseCurveRef = TEXT("/Game/ProjectHive/Curve/Curve_Tension_Increase.Curve_Tension_Increase");
	if (TensionIncreaseCurveRef.Object != nullptr)
	{
		TensionIncreaseCurve = TensionIncreaseCurveRef.Object;
	}

	// Load RotationCurve
	static ConstructorHelpers::FObjectFinder<UCurveFloat> RotationCurveRef = TEXT("/Game/ProjectHive/Curve/Curve_TurnRotation.Curve_TurnRotation");
	if (RotationCurveRef.Object != nullptr)
	{
		RotationCurve = RotationCurveRef.Object;
	}

	// Load ChangeWeaponMontage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ChangeWeaponRef = TEXT("/Game/ProjectHive/Animation/AM_ChangeWeapon.AM_ChangeWeapon");
	if (ChangeWeaponRef.Object != nullptr)
	{
		ChangeWeaponMontage = ChangeWeaponRef.Object;
	}
}

void APHPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 이거 따로 뺄지 고민해 보기
	PlayerController = Cast<APHPlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	if (PlayerController != nullptr)
	{
		InteractComponent->OnInteractTargetOn.BindUObject(PlayerController, &APHPlayerController::ShowInteractUI);
		InteractComponent->OnInteractTargetOff.BindUObject(PlayerController, &APHPlayerController::HideInteractUI);
	}

	// Setting Movement
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = StatComponent->WalkMovementSpeed;

	// Setting Timeline
	InitializeTimeLine();
}

void APHPlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Setting Timeline
	TensionDecreaseTimeline.TickTimeline(DeltaTime);
	TensionIncreaseTimeline.TickTimeline(DeltaTime);
	SettingRotationTimeline.TickTimeline(DeltaTime);
	if (bIsGrenadeAiming || bIsAiming)
	{
		UpdateCharacterRotator(DeltaTime);
	}
}

void APHPlayableCharacter::InitializeTimeLine()
{
	// 경계레벨 감소용 함수 바인딩
	FOnTimelineFloat TensionDecreaseUpdate;
	TensionDecreaseUpdate.BindUFunction(this, FName("DecreaseTensionLevel"));

	TensionDecreaseTimeline.AddInterpFloat(TensionDecreaseCurve, TensionDecreaseUpdate);

	// 경계레벨 증가용 함수 바인딩
	FOnTimelineFloat TensionIncreaseUpdate;
	TensionIncreaseUpdate.BindUFunction(this, FName("IncreaseTensionLevel"));

	TensionIncreaseTimeline.AddInterpFloat(TensionIncreaseCurve, TensionIncreaseUpdate);

	FOnTimelineFloat ActorRotationUpdate;
	ActorRotationUpdate.BindUFunction(this, FName("SetActorToTargetRotation"));

	SettingRotationTimeline.AddInterpFloat(RotationCurve, ActorRotationUpdate);

}

void APHPlayableCharacter::InitializeActionMappings()
{
	ActionMapping.Add(ECharacterActionType::MoveAction, &APHPlayableCharacter::Move);
	ActionMapping.Add(ECharacterActionType::InteractAction, &APHPlayableCharacter::Interact);
	ActionMapping.Add(ECharacterActionType::DropWeapon, &APHPlayableCharacter::DropWeapon);
	ActionMapping.Add(ECharacterActionType::Attack, &APHPlayableCharacter::Attack);
	ActionMapping.Add(ECharacterActionType::AimStart, &APHPlayableCharacter::AimStart);
	ActionMapping.Add(ECharacterActionType::AimHold, &APHPlayableCharacter::AimHold);
	ActionMapping.Add(ECharacterActionType::AimEnd, &APHPlayableCharacter::AimEnd);
	ActionMapping.Add(ECharacterActionType::SwapWeapon, &APHPlayableCharacter::SwapWeapon);
	ActionMapping.Add(ECharacterActionType::RunStart, &APHPlayableCharacter::RunStart);
	ActionMapping.Add(ECharacterActionType::RunEnd, &APHPlayableCharacter::RunEnd);
	ActionMapping.Add(ECharacterActionType::Reload, &APHPlayableCharacter::Reload);
	ActionMapping.Add(ECharacterActionType::ThrowGrenade, &APHPlayableCharacter::ThrowGrenade);
	ActionMapping.Add(ECharacterActionType::AimGrenade, &APHPlayableCharacter::UpdateGrenadeAimDirection);
	ActionMapping.Add(ECharacterActionType::AimGrenadeStart, &APHPlayableCharacter::BeginGrenadeAimDirection);
	
}

void APHPlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UE_LOG(LogTemp, Log, TEXT("SetupPlayerInputComponent"));

	SetMappingContext();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	BindInputAction(EnhancedInputComponent);
}

void APHPlayableCharacter::Attack()
{
	// 달리기 중이면 공격불가
	if (bIsRunning)
	{
		return;
	}
	// 타임라인 재생
	if (!TensionIncreaseTimeline.IsPlaying() && TensionLevel != MaxTensionLevel)
	{
		TensionIncreaseTimeline.Play();
	}

	if (WeaponComponent != nullptr && WeaponComponent->CanAttack() && TensionLevel == MaxTensionLevel)
	{
		if (!bIsAttacking)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				bIsAttacking = true;
				// 조준 여부에 따라 호출하는 몽타주 다르게 하기
				if (IsAiming())
				{
					AttackActionMontage = WeaponComponent->GetNormalAimAttackMontage();
				}
				else
				{
					AttackActionMontage = WeaponComponent->GetNormalAttackMontage();
				}
				if (AttackActionMontage != nullptr)
				{
					// 몽타주 끝날때 실행되는 델리게이트, 애님 몽타주랑, 불값을 인수로 가짐
					FOnMontageEnded EndDelegate;
					EndDelegate.BindUObject(this, &APHPlayableCharacter::OnAttackActionMontageEnd);

					AnimInstance->Montage_Play(AttackActionMontage, 0.9f);
					AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackActionMontage);
				}
				
			}
		}
		WeaponComponent->Attack();
	}
}

void APHPlayableCharacter::ThrowGrenade()
{
	if (!UpdateMouseLocation())
	{
		UE_LOG(LogTemp, Log, TEXT("Not Update MouseLocation"));
	}
	
	GrenadeComponent->ThrowGrenade(GetMesh(), GreandeTarget);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bIsGrenadeAiming = false;
}

float APHPlayableCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	StatComponent->ChangeHP(Damage);
	return 0.0f;
}

void APHPlayableCharacter::SetUpHUD(UPHHUDWidget* InHUDWidget)
{
	if (InHUDWidget == nullptr)
	{
		return;
	}

	// HP 업데이트
	InHUDWidget->UpdateHP(StatComponent->HpPersent);

	StatComponent->OnHPChange.AddUObject(InHUDWidget, &UPHHUDWidget::UpdateHP);

	// 여기서 무기UI업데이트 필요
	WeaponComponent->OnGunEquipped.AddUObject(InHUDWidget, &UPHHUDWidget::SetupGunUI);
	WeaponComponent->SetWeapon(GetWeaponComponent()->GetWeapon());

	WeaponComponent->OnGunUpdate.AddUObject(InHUDWidget, &UPHHUDWidget::UpdateGunUI);

	// 무기 버리는거 업데이트
}

bool APHPlayableCharacter::GetIsDead() const
{
	return bIsDead;
}

void APHPlayableCharacter::BindInputAction(UEnhancedInputComponent* InEnhancedInputComponent)
{
	for (const auto& pair : CharacterInputActionData->InputBindings)
	{
		auto ActionType = pair.Key;
		auto Action = pair.Value.InputAction;
		if (Action == nullptr)
		{
			continue;
		}
		auto TriggerEvent = pair.Value.TriggerEvent;

		if (const auto* MappingFunc = ActionMapping.Find(ActionType))
		{
			InEnhancedInputComponent->BindAction(Action, TriggerEvent, this, *MappingFunc);
		}
	}
}

void APHPlayableCharacter::Move(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();

	// TODO : Request Update
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = Movement.SizeSquared();

	// Normalize size when input is two direction
	if (MovementVectorSizeSquared > 1.0f)
	{
		Movement.Normalize();
		MovementVectorSize = 1.0f;
	}
	// if input's scale under one, calculate scale to use 
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(Movement.X, Movement.Y, 0.0f);

	AddMovementInput(MoveDirection, MovementVectorSize);
}

void APHPlayableCharacter::Interact(const FInputActionValue& Value)
{
	if (InteractComponent != nullptr)
	{	
		// 서버 RPC
		InteractComponent->Interact();
	}
}

void APHPlayableCharacter::DropWeapon(const FInputActionValue& Value)
{
	if (APHEquipment* Weapon = WeaponComponent->GetWeapon())
	{
		EquipmentComponent->DropEquipment(Weapon);
	}
}

void APHPlayableCharacter::SetMappingContext()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	if (CharacterInputActionData != nullptr)
	{
		DefaultMappingContext = CharacterInputActionData->CharacterInputMapping;
	}

	if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

bool APHPlayableCharacter::UpdateMouseLocation()
{
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PLayerController is nullptr"))
		return false;
	}

	FHitResult HitResult;
	// TODO : 공중 같은 곳에 에임을 할때 발생하는 문제를 처리하기 위해 Collision을 하나 맵 전체에 덮을 예정
	// 마우스가 몬스터 같은 공격 가능한 액터를 hit할경우 따로 분기 처리 필요
	const bool Hit = PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	if (Hit && HitResult.bBlockingHit)
	{
		MouseLocation = HitResult.Location;
		return true;
	}
	return false;
}

void APHPlayableCharacter::SetMovementSpeed()
{
	if (bIsAiming)
	{
		GetCharacterMovement()->MaxWalkSpeed = StatComponent->AimMovementSpeed;
	}
	else if (bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = StatComponent->RunMovementSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = StatComponent->WalkMovementSpeed;
	}
}

void APHPlayableCharacter::DecreaseTensionLevel(float Value)
{
	TensionLevel = MaxTensionLevel * Value;
}

void APHPlayableCharacter::IncreaseTensionLevel(float Value)
{
	
	TensionLevel = MaxTensionLevel * Value;
}

void APHPlayableCharacter::OnAttackActionMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackActionMontage)
	{
		bIsAttacking = false;
	}
}

void APHPlayableCharacter::StartIncreaseTinesionFromCurrentLevel()
{
	if (!TensionIncreaseTimeline.IsPlaying())
	{
		float Length = TensionIncreaseCurve->FloatCurve.GetLastKey().Time;
		float TensionLevelStart = (TensionLevel / MaxTensionLevel) * Length;
		TensionIncreaseTimeline.SetPlaybackPosition(TensionLevelStart, false, false);
		TensionIncreaseTimeline.SetLooping(false);
		TensionIncreaseTimeline.Play();
	}
}

void APHPlayableCharacter::SetActorToTargetRotation(float Value)
{
	if (!UpdateMouseLocation())
	{
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Set"));
	const FVector ToMouse = MouseLocation - GetActorLocation();
	FRotator CurrentRotation = GetActorRotation();
	const FRotator NewRotation = FRotator(CurrentRotation.Pitch, ToMouse.Rotation().Yaw, CurrentRotation.Roll);
	FRotator TargetRotation = FMath::Lerp(CurrentRotation, NewRotation, Value);
	SetActorRotation(TargetRotation);
}

void APHPlayableCharacter::Attack(const FInputActionValue& Value)
{
	Attack();
}

void APHPlayableCharacter::AimStart(const FInputActionValue& Value)
{
	// 달리는 중이면 조준 불가
	if (bIsRunning)
	{
		return;
	}

	GetCharacterMovement()->bOrientRotationToMovement = false;
	bIsAiming = true;
	// 이동속도 조절
	GetCharacterMovement()->MaxWalkSpeed = StatComponent->AimMovementSpeed;

	// 회전 타임라인 초기화
	SettingRotationTimeline.SetPlaybackPosition(0.0f, false, false);
}

void APHPlayableCharacter::AimHold(const FInputActionValue& Value)
{
	if (bIsRunning)
	{
		return;
	}
	bIsAiming = true;

	/*SettingRotationTimeline.SetLooping(false);
	SettingRotationTimeline.PlayFromStart();*/
	// 마우스 위치 받아오기
	//if (UpdateMouseLocation())
	//{
	//	// 마우스 위치 - 액터 위치 -> 방향 벡터 계산
	//	const FVector ToMouse = MouseLocation - GetActorLocation();

	//	if (!ToMouse.IsNearlyZero())
	//	{
	//		FRotator CurrentRotation = GetActorRotation();
	//		const FRotator NewRotation = FRotator(CurrentRotation.Pitch, ToMouse.Rotation().Yaw, CurrentRotation.Roll);
	//		SetActorRotation(NewRotation);	
	//	}
	//}
}

void APHPlayableCharacter::AimEnd(const FInputActionValue& Value)
{
	SettingRotationTimeline.Stop();
	bIsAiming = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// 이동속도 조절
	SetMovementSpeed();
}

void APHPlayableCharacter::SwapWeapon(const FInputActionValue& Value)
{
	// 무기 컴포넌트에서 현재 무기와 보조 무기의 위치를 바꿈
	// 마우스 휠로 값을 받아옴
	float ScrollValue = Value.Get<float>();

	// 방향 설정
	if (ScrollValue > 0.0f)
	{
		SwapDirection = 1;
	}
	else if(ScrollValue < 0.0f)
	{
		SwapDirection = -1;
	}

	// 유요한지 방향인지 확인
	if (SwapDirection == 0)
	{
		return;
	}

	// 스왑이 가능한지 확인
	/*if (!EquipmentComponent->CanSwapWeapon(SwapDirection))
	{
		return;
	}*/

	// 몽타주 실행
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		// TODO : 몽타주 실행중 다시 실행하지 말라고 플래그값 세우기
		AnimInstance->Montage_Play(ChangeWeaponMontage, 1.2f);
	}
}

void APHPlayableCharacter::RunStart(const FInputActionValue& Value)
{
	bIsRunning = true;
	// 달리기 시작하면 조준 끝내기
	if (bIsAiming)
	{
		AimEnd(Value);
	}

	// 경계레벨 0으로 만들기
	TensionLevel = MinTensionLevel;
	SetMovementSpeed();
}

void APHPlayableCharacter::RunEnd(const FInputActionValue& Value)
{
	bIsRunning = false;
	if (bIsAiming)
	{
		TensionLevel = MaxTensionLevel;
	}

	SetMovementSpeed();
}

void APHPlayableCharacter::Reload(const FInputActionValue& Value)
{
	if (WeaponComponent == nullptr)
	{
		return;
	}

	if (WeaponComponent->CanReload())
	{
		UAnimMontage* ReloadMontage = WeaponComponent->GetReloadMontage();
		if (ReloadMontage == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("ReloadMontage is nullptr"));
			return;
		}

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			// 몽타주 실행중 다시 실행하지 말라고 플래그값 세우기
			WeaponComponent->ReloadStart();

			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(WeaponComponent, &UPHWeaponComponent::ReloadEnd);

			AnimInstance->Montage_Play(ReloadMontage, 1.2f);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, ReloadMontage);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Can't Reload"));
	}
}

void APHPlayableCharacter::ThrowGrenade(const FInputActionValue& Value)
{
	if (bIsRunning || !bIsGrenadeAiming)
	{
		return;
	}
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Not ThrowGrenade"));
		return;
	}

	GreandeTarget = MouseLocation;
	if (!AnimInstance->Montage_IsPlaying(WeaponComponent->GetThrowMontage()))
	{
		AnimInstance->Montage_Play(WeaponComponent->GetThrowMontage());
	}
}

void APHPlayableCharacter::UpdateGrenadeAimDirection(const FInputActionValue& Value)
{

}

void APHPlayableCharacter::BeginGrenadeAimDirection(const FInputActionValue& Value)
{
	if (WeaponComponent == nullptr || WeaponComponent->GetThrowMontage() == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ThrowMontage is nullptr"));
		return;
	}
	
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bIsGrenadeAiming = true;
	//if (UpdateMouseLocation())
	//{
	//	// 마우스 위치 - 액터 위치 -> 방향 벡터 계산
	//	const FVector ToMouse = MouseLocation - GetActorLocation();

	//	if (!ToMouse.IsNearlyZero())
	//	{
	//		FRotator CurrentRotation = GetActorRotation();
	//		float NewYaw = FMath::Lerp(CurrentRotation.Yaw, ToMouse.Rotation().Yaw, 0.0f);
	//		const FRotator TargetRotation = FRotator(CurrentRotation.Pitch, NewYaw, CurrentRotation.Roll);
	//		SetActorRotation(TargetRotation);
	//	}
	//}
}

void APHPlayableCharacter::PickupItem(APHItem* InItem)
{
	// Item은 타입을 가지고 있어서 그것으로 분기
	// 컴포넌트에 넘겨줘고 그 타입이라는 믿음 아래에서 시작하는 방식으로 함
	if (InItem == nullptr)
	{
		return;
	}

	if (InItem->GetItemType() == EItemType::Equipment)
	{
		EquipmentComponent->Equip(InItem, GetMesh());
	}

	if (InItem->GetItemType() == EItemType::Consumeable)
	{
		UE_LOG(LogTemp, Log, TEXT("Get ConsumeItem"));
	}
}

void APHPlayableCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	WeaponComponent->InitializeWeaponMesh(GetMesh());

	// Setting EquipmentSlots
	EquipmentComponent->SetEquipmentSlot();

	// Setting Interact Overlap Delegate
	InteractTrigger->OnComponentBeginOverlap.AddDynamic(InteractComponent.Get(), &UPHInteractComponent::OnInteractableBeginOverlap);
	InteractTrigger->OnComponentEndOverlap.AddDynamic(InteractComponent.Get(), &UPHInteractComponent::OnInteractableEndOverlap);

	// 무기컴포넌트에 알려주기 위한 바인딩
	EquipmentComponent->OnEquipmentEquipped.AddUObject(WeaponComponent.Get(), &UPHWeaponComponent::SetWeapon);
	EquipmentComponent->OnEquipmentUnequipped.AddUObject(WeaponComponent.Get(), &UPHWeaponComponent::ClearWeapon);

	// 체력이 0이면 죽는 몽타주 실행
	StatComponent->OnDead.AddUObject(this, &APHPlayableCharacter::Die);
}

void APHPlayableCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PlayerController = CastChecked<APHPlayerController>(NewController);
}



void APHPlayableCharacter::UpdateCharacterRotator(float DeltaTime)
{
	if (UpdateMouseLocation())
	{
		// 마우스 위치 - 액터 위치 -> 방향 벡터 계산
		const FVector ToMouse = MouseLocation - GetActorLocation();

		if (!ToMouse.IsNearlyZero())
		{
			FRotator CurrentRotation = GetActorRotation();
			const FRotator TargetRotation = FRotator(CurrentRotation.Pitch, ToMouse.Rotation().Yaw, CurrentRotation.Roll);
			const FRotator NewRot = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 10.0f);
			SetActorRotation(NewRot);
			if (CurrentRotation.Equals(TargetRotation, 1.0f))
			{
				bIsGrenadeAiming = false;
			}
		}
	}
}

void APHPlayableCharacter::Die()
{
	bIsDead = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	DisableInput(PlayerController);
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("AnimInstance is nullptr : Die"));
		return;
	}

	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void APHPlayableCharacter::OnDeferredWeaponEquipped()
{
	// 무기 컴포넌트에 가서 무기의 애님인스턴스 가져오기
	if (WeaponComponent->GetWeapon() == nullptr)
	{
		GetMesh()->SetAnimClass(CharacterAnim);
		return;
	}
	GetMesh()->SetAnimClass(WeaponComponent->GetWeapon()->GetWeaponAnimClass());
}

void APHPlayableCharacter::OnWeaponEquipped()
{
	// PostAnimEvaluation() 재귀 호출로 인한 크러시를 방지하기 위해 다음 프레임에서 캐릭터 애니메이션 변경 
	GetWorldTimerManager().SetTimerForNextTick(this, &APHPlayableCharacter::OnDeferredWeaponEquipped);
}

void APHPlayableCharacter::OnWeaponUnequipped()
{

}

void APHPlayableCharacter::ChangeWeapon()
{
	EquipmentComponent->SwapWeapon(SwapDirection);
	SwapDirection = 0;
}
