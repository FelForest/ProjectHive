// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHPlayableCharacter.h"

#include "Weapon/PHWeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

//#include "Input/PHCharacterInputActionData.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


APHPlayableCharacter::APHPlayableCharacter()
{
	// Setting Weapon 
	Weapon = CreateDefaultSubobject<UPHWeaponComponent>(TEXT("WeaponComponent"));

	// Need to Setting before AnimInstance Setting
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh());
	WeaponMesh->SetLeaderPoseComponent(GetMesh());

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

void APHPlayableCharacter::SetEquipment()
{
	//WeaponMesh->SetSkeletalMesh()
	if (Weapon != nullptr)
	{
		Weapon->SetWeapon();
	}
}

void APHPlayableCharacter::Attack()
{
	if (Weapon != nullptr)
	{
		Weapon->Attack();
	}
}


