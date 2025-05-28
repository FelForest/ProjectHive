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

// �ӽÿ�
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
	// TODO : �����ѹ� �����ͷ� �޾ƿ���
	InteractTrigger->SetSphereRadius(98.5f);

	// Setting Action
	ActionMapping.Add(ECharacterActionType::MoveAction, &APHPlayableCharacter::Move);
	// TODO : Interactable �������̽� ������� ��
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
		// ���� RPC
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

// �̰͵� ������Ʈ�� ������ �ҵ�
void APHPlayableCharacter::SetEquipment(APHEquipment* InEquipment)
{
	// ��� ĳ������ SkeletalComponent�� ���̱�
	//	�ִϸ��̼� ����ȭ�� ���� ����
	InEquipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	InEquipment->GetEquipmentMesh()->SetLeaderPoseComponent(GetMesh());
	InEquipment->OnPickup();
	//WeaponMesh->SetSkeletalMesh()
	// ���⼭ ��������Ʈ �ϴ°� ����
	// ��� ���̷��� �޽õ� ���⼭ �ϴ°� �´µ�
	if (WeaponComponent != nullptr)
	{
		// �Ķ���ͷ� InEquipment �Ѱ������
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
	// �ڵ� �ܿ��� �ϴ°� �´��� �����ܿ��� Ȯ���ϴ°� �´��� �𸣰���
	// ����� ���⳪ ��� �̷��� ũ�� ���� ������ �Ǵ��� ��

	// ��� ������ ���������� �ƴ��� �Ǵ�
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


