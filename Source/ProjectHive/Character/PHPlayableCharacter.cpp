﻿// Fill out your copyright notice in the Description page of Project Settings.


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

// OverLap Section
#include "Components/SphereComponent.h"

// 임시용
#include "Player/PHPlayerController.h"



APHPlayableCharacter::APHPlayableCharacter()
{
	// Tick 활성화
	PrimaryActorTick.bCanEverTick = true;

	// Setting Weapon 
	WeaponComponent = CreateDefaultSubobject<UPHWeaponComponent>(TEXT("WeaponComponent"));

	// Setting Equipment
	EquipmentComponent = CreateDefaultSubobject<UPHEquipmentComponent>(TEXT("EquipmentComponent"));

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

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// 이게 가능한 이유 : 맴버 변수로 가지고 있어서 이게 먼저 초기화되는게 보장됨
	GetCharacterMovement()->bOrientRotationToMovement = true;
	AimSpeed = 250.0f;
	WalkSpeed = 500.0f;
	RunSpeed = 750.0f;
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
	static ConstructorHelpers::FClassFinder<UAnimInstance> CharacterAnim(TEXT("/Game/ProjectHive/Animation/NewFolder/ABP_AR.ABP_AR_C"));
	if (CharacterAnim.Class)
	{
		GetMesh()->SetAnimClass(CharacterAnim.Class);
	}

	// Load CharacterInputActionData
	static ConstructorHelpers::FObjectFinder<UPHCharacterInputActionData> CharacterInputActionDataRef = TEXT("/Script/ProjectHive.PHCharacterInputActionData'/Game/ProjectHive/Input/PHC_InputActionData.PHC_InputActionData'");
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
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// Setting Timeline
	InitializeTimeLine();
}

void APHPlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Setting Timeline
	TensionDecreaseTimeline.TickTimeline(DeltaTime);
	TensionIncreaseTimeline.TickTimeline(DeltaTime);

	UE_LOG(LogTemp, Log, TEXT("%f"), TensionLevel);
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
}

void APHPlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetMappingContext();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	BindInputAction(EnhancedInputComponent);
}

void APHPlayableCharacter::Attack()
{
	// 타임라인 재생
	TensionIncreaseTimeline.SetLooping(false);
	TensionDecreaseTimeline.Stop();
	TensionIncreaseTimeline.Play();
	if (WeaponComponent != nullptr && TensionLevel == MaxTensionLevel)
	{
		WeaponComponent->Attack();
	}
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
	

	/*InEnhancedInputComponent->BindAction(
		CharacterInputActionData->InputBindings[ECharacterActionType::MoveAction].InputAction, 
		CharacterInputActionData->InputBindings[ECharacterActionType::MoveAction].TriggerEvent,
		this, &APHPlayableCharacter::Move);

	InEnhancedInputComponent->BindAction(
		CharacterInputActionData->InputBindings[ECharacterActionType::InteractAction].InputAction,
		CharacterInputActionData->InputBindings[ECharacterActionType::InteractAction].TriggerEvent,
		this, &APHPlayableCharacter::Interact);*/
	
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

	APHPlayerController* PHPlayerController = CastChecked<APHPlayerController>(GetController());
	if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PHPlayerController->GetLocalPlayer()))
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
		GetCharacterMovement()->MaxWalkSpeed = AimSpeed;
	}
	else if (bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void APHPlayableCharacter::DecreaseTensionLevel(float Value)
{
	TensionLevel = Value * 100.0f;
}

void APHPlayableCharacter::IncreaseTensionLevel(float Value)
{
	TensionLevel = Value * 100.0f;
}

void APHPlayableCharacter::Attack(const FInputActionValue& Value)
{

	Attack();
}

void APHPlayableCharacter::AimStart(const FInputActionValue& Value)
{
	TensionLevel = MaxTensionLevel;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bIsAiming = true;
	// 이동속도 조절
	GetCharacterMovement()->MaxWalkSpeed = AimSpeed;
}

void APHPlayableCharacter::AimHold(const FInputActionValue& Value)
{
	// 마우스 위치 받아오기
	if (UpdateMouseLocation())
	{
		// 마우스 위치 - 액터 위치 -> 방향 벡터 계산
		const FVector ToMouse = MouseLocation - GetActorLocation();
		// 방향만 보는 용도여서 정규화 필요 없음
		// 캐릭터 Rotator Yaw 만 변경

		// 마우스 방향이 무시할 정도가 아니라는 것
		if (!ToMouse.IsNearlyZero())
		{
			FRotator CurrentRotation = GetActorRotation();
			const FRotator NewRotation = FRotator(CurrentRotation.Pitch, ToMouse.Rotation().Yaw, CurrentRotation.Roll);
			SetActorRotation(NewRotation);	
		}
	}


}

void APHPlayableCharacter::AimEnd(const FInputActionValue& Value)
{
	TensionDecreaseTimeline.SetLooping(false);
	TensionDecreaseTimeline.PlayFromStart();
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

	int32 Direction = 0;
	// 양수
	if (ScrollValue > 0.0f)
	{
		Direction = 1;
	}
	// 음수
	else if(ScrollValue < 0.0f)
	{
		Direction = -1;
	}

	if (Direction != 0)
	{
		EquipmentComponent->SwapWeapon(Direction);
	}
}

void APHPlayableCharacter::RunStart(const FInputActionValue& Value)
{
	bIsRunning = true;
	SetMovementSpeed();
}

void APHPlayableCharacter::RunEnd(const FInputActionValue& Value)
{
	bIsRunning = false;
	SetMovementSpeed();
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

	// 장비에 대한 UI 바인딩

}

void APHPlayableCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}
