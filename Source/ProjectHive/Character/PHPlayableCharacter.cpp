// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHPlayableCharacter.h"

// Camera Setction
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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
	// Setting Weapon 
	WeaponComponent = CreateDefaultSubobject<UPHWeaponComponent>(TEXT("WeaponComponent"));

	// Setting Equipment
	EquipmentComponent = CreateDefaultSubobject<UPHEquipmentComponent>(TEXT("EquipmentComponent"));

	// Setting AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> CharacterAnim(TEXT("/Game/ProjectHive/Animation/ABP_AR.ABP_AR_C"));
	if (CharacterAnim.Class)
	{
		GetMesh()->SetAnimClass(CharacterAnim.Class);
	}

	// Setting Input
	/*static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Game/ProjectHive/Input/IMC_Player.IMC_Player"));
	if (InputMappingContextRef.Object != nullptr)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}*/

	static ConstructorHelpers::FObjectFinder<UPHCharacterInputActionData> CharacterInputActionDataRef = TEXT("/Script/ProjectHive.PHCharacterInputActionData'/Game/ProjectHive/Input/PHC_InputActionData.PHC_InputActionData'");
	if (CharacterInputActionDataRef.Object != nullptr)
	{
		CharacterInputActionData = CharacterInputActionDataRef.Object;
	}

	SetReplicates(true);
	// Setting InputAction
	MoveAction = CharacterInputActionData->InputBindings[ECharacterActionType::MoveAction].InputAction;
	InteractAction = CharacterInputActionData->InputBindings[ECharacterActionType::MoveAction].InputAction;

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

	// Setting InteractComponent
	InteractComponent = CreateDefaultSubobject<UPHInteractComponent>(TEXT("InteractComponent"));

	// Setting InteractTrigger
	InteractTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("InteractTrigger"));
	InteractTrigger->SetupAttachment(RootComponent);
	InteractTrigger->SetCollisionProfileName(TEXT("Interaction"));
	// TODO : 매직넘버 데이터로 받아오기 변경
	InteractTrigger->SetSphereRadius(98.5f);

	// Setting Action
	ActionMapping.Add(ECharacterActionType::MoveAction, &APHPlayableCharacter::Move);
	ActionMapping.Add(ECharacterActionType::InteractAction, &APHPlayableCharacter::Interact);
	ActionMapping.Add(ECharacterActionType::DropWeapon, &APHPlayableCharacter::DropWeapon);
	ActionMapping.Add(ECharacterActionType::Attack, &APHPlayableCharacter::Attack);
	ActionMapping.Add(ECharacterActionType::Aim, &APHPlayableCharacter::Aim);
}

void APHPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

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
	if (WeaponComponent != nullptr)
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

void APHPlayableCharacter::Attack(const FInputActionValue& Value)
{
	Attack();
}

void APHPlayableCharacter::Aim(const FInputActionValue& Value)
{
	if (PlayerController == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Aiming"));
	// 마우스 위치 받아오기
	FVector WorldOrigin, WorldDirection;
	if (PlayerController->DeprojectMousePositionToWorld(WorldOrigin, WorldDirection))
	{
		// 마우스 방향으로 멀리 직선 연장
		FVector TraceEnd = WorldOrigin + WorldDirection * 10000.f;

		// 캐릭터 → 마우스 방향 벡터
		FVector ToCursor = TraceEnd - GetActorLocation();
		

		if (!ToCursor.IsNearlyZero())
		{
			FRotator TargetRot = ToCursor.Rotation();

			// Pitch, Roll 제거 (Yaw만 회전)
			SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
		}
	}
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


