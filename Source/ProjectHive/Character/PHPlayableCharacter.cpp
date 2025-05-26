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

// Item Including Section
#include "Item/PHItem.h"
#include "Item/Equipment/PHEquipment.h"

#include "Components/CapsuleComponent.h"



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

	// Setting Camera
	// TODO : Consider moving to an attackable character
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	// Update later
	// length, angle, etc...
	SpringArm->TargetArmLength = 1400.0f;
	SpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Setting Action
	ActionMapping.Add(ECharacterActionType::MoveAction, &APHPlayableCharacter::Move);
	// TODO : Interactable �������̽� ������� ��
	//ActionMapping.Add(ECharacterActionType::InteractAction,)
}

void APHPlayableCharacter::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	// ����� �̱������� ��Ƽ�� ��� Ȯ���ؾ��� ������ ����
	// ���� ���� ��Ʈ�ѷ� ���� Ȯ��
	// �����̸� �������� UI�� Ȱ��ȭ

	// ������ ���������� �ϸ� �Ǵ°�
	// ��ȣ�ۿ� �׼��� ���� �����ϰ� �ϸ� �Ǵ°�
	// �̰��� true�̸� ������ ȹ�氡��

	// �ϴ� ��θ� �Ѱɷ� �ϰ� ���� �̱۷�
	APHItem* Item = Cast<APHItem>(OtherActor);

	// ���� �ܿ��� Ȯ���� ������ Ȥ�ó� �ؼ� Ȯ�� -> ��κ� �޸� ����
	if (Item == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("ItemBeginOverLap"));

	Item->ShowUI();
}
void APHPlayableCharacter::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Ŭ��
	// �������� UI�� ���� ���
	// ĳ���Ͱ� ������ ȹ�� �ϰ� �ݸ����� ������ �� �Լ� ȣ��
	// �Ѵ� �ϸ� �Ѹ��� Ű���� �ٸ� �Ѹ��� ������ ������ ��������

	// ����
	// ��ȣ�ۿ� �׼� ���� ���� false�� ����

	UE_LOG(LogTemp, Log, TEXT("ItemEndOverLap"));
}
void APHPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CharacterInputActionData != nullptr)
	{
		DefaultMappingContext = CharacterInputActionData->CharacterInputMapping;
	}

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void APHPlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	BindInputAction(EnhancedInputComponent);
}

void APHPlayableCharacter::BindInputAction(UEnhancedInputComponent* InEnhancedInputComponent)
{
	if (CharacterInputActionData == nullptr)
	{
		return;
	}

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

void APHPlayableCharacter::Interact(AActor* InInteractor)
{
	IPHInteractableInterface* Interactable = Cast<IPHInteractableInterface>(InInteractor);

	if (Interactable == nullptr)
	{
		return;
	}

	Interactable->Interact();
}

void APHPlayableCharacter::SetEquipment(APHEquipment* InEquipment)
{
	// ��� ĳ������ SkeletalComponent�� ���̱�
	//	�ִϸ��̼� ����ȭ�� ���� ����
	InEquipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	InEquipment->GetEquipmentMesh()->SetLeaderPoseComponent(GetMesh());
	//WeaponMesh->SetSkeletalMesh()
	// ���⼭ ��������Ʈ �ϴ°� ����
	// ��� ���̷��� �޽õ� ���⼭ �ϴ°� �´µ�
	if (WeaponComponent != nullptr)
	{
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

	UCapsuleComponent* Trigger = Cast<UCapsuleComponent>(RootComponent);

	if (Trigger != nullptr)
	{
		Trigger->OnComponentBeginOverlap.AddDynamic(this, &APHPlayableCharacter::OnItemBeginOverlap);
		Trigger->OnComponentEndOverlap.AddDynamic(this, &APHPlayableCharacter::OnItemEndOverlap);
	}
}


