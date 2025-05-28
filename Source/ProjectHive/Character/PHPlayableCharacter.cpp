// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHPlayableCharacter.h"

#include "Weapon/PHWeaponComponent.h"
//#include "Components/SkeletalMeshComponent.h"

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
#include "Item/Equipment/Weapon/PHWeapon.h"

#include "Components/SphereComponent.h"

// 임시용
#include "Player/PHPlayerController.h"



APHPlayableCharacter::APHPlayableCharacter()
{
	// Setting Weapon 
	WeaponComponent = CreateDefaultSubobject<UPHWeaponComponent>(TEXT("WeaponComponent"));

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
	// TODO : 매직넘버 데이터로 받아오기
	InteractTrigger->SetSphereRadius(98.5f);

	// Setting Action
	ActionMapping.Add(ECharacterActionType::MoveAction, &APHPlayableCharacter::Move);
	// TODO : Interactable 인터페이스 만들어진 후
	ActionMapping.Add(ECharacterActionType::InteractAction, &APHPlayableCharacter::Interact);
}

void APHPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	APHPlayerController* PHPlayerController = Cast<APHPlayerController>(GetController());
	if (PHPlayerController)
	{
		EnableInput(PHPlayerController);
	}

	if (PHPlayerController != nullptr)
	{
		InteractComponent->OnInteractTargetOn.BindUObject(PHPlayerController, &APHPlayerController::ShowInteractUI);
		InteractComponent->OnInteractTargetOff.BindUObject(PHPlayerController, &APHPlayerController::HideInteractUI);
	}
}

void APHPlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetMappingContext();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	BindInputAction(EnhancedInputComponent);
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

// 이것도 컴포넌트로 조개야 할듯
void APHPlayableCharacter::SetEquipment(APHEquipment* InEquipment)
{
	// 장비를 캐릭터의 SkeletalComponent에 붙이기
	//	애니메이션 동기화를 위한 세팅
	InEquipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	InEquipment->GetEquipmentMesh()->SetLeaderPoseComponent(GetMesh());
	InEquipment->OnPickup();
	//WeaponMesh->SetSkeletalMesh()
	// 여기서 델리게이트 하는게 맞음
	// 장비 스켈레톤 메시도 여기서 하는게 맞는듯
	if (WeaponComponent != nullptr)
	{
		// 파라미터로 InEquipment 넘겨줘야함
		WeaponComponent->SetWeapon();
	}
}

void APHPlayableCharacter::Attack()
{
	if (WeaponComponent != nullptr)
	{
		WeaponComponent->Attack();
	}
}

void APHPlayableCharacter::PickupItem(APHItem* InItem)
{
	// 코드 단에서 하는게 맞는지 엔진단에서 확인하는게 맞는지 모르겠음
	// 현재는 무기나 장비 이런게 크게 없을 것으로 판단이 됨

	// 장비 가능한 아이템인지 아닌지 판단
	APHEquipment* Equipment = Cast<APHEquipment>(InItem);
	if (Equipment != nullptr)
	{
		SetEquipment(Equipment);
	}
}

void APHPlayableCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	WeaponComponent->InitializeWeaponMesh(GetMesh());

	// Setting Interact Overlap Delegate
	InteractTrigger->OnComponentBeginOverlap.AddDynamic(InteractComponent.Get(), &UPHInteractComponent::OnInteractableBeginOverlap);
	InteractTrigger->OnComponentEndOverlap.AddDynamic(InteractComponent.Get(), &UPHInteractComponent::OnInteractableEndOverlap);
}

void APHPlayableCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	

	
}


