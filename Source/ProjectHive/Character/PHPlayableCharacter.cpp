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
	// TODO : Interactable 인터페이스 만들어진 후
	//ActionMapping.Add(ECharacterActionType::InteractAction,)
}

void APHPlayableCharacter::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	// 현재는 싱글이지만 멀티인 경우 확인해야할 로직이 있음
	// 먼저 로컬 컨트롤러 인지 확인
	// 로컬이면 아이템의 UI를 활성화

	// 다음은 서버에서만 하면 되는것
	// 상호작용 액션의 값을 수정하게 하면 되는것
	// 이값이 true이면 아이템 획득가능

	// 일단 고민만 한걸로 하고 먼저 싱글로
	APHItem* Item = Cast<APHItem>(OtherActor);

	// 엔진 단에서 확인은 하지만 혹시나 해서 확인 -> 대부분 휴먼 오류
	if (Item == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("ItemBeginOverLap"));

	Item->ShowUI();
}
void APHPlayableCharacter::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 클라
	// 아이템의 UI를 끄는 방식
	// 캐릭터가 아이템 획득 하고 콜리전이 꺼지면 이 함수 호출
	// 둘다 하면 한명이 키고나서 다른 한명이 들어오고 나가면 꺼져버림

	// 서버
	// 상호작용 액션 가능 값을 false로 변경

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
	// 장비를 캐릭터의 SkeletalComponent에 붙이기
	//	애니메이션 동기화를 위한 세팅
	InEquipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	InEquipment->GetEquipmentMesh()->SetLeaderPoseComponent(GetMesh());
	//WeaponMesh->SetSkeletalMesh()
	// 여기서 델리게이트 하는게 맞음
	// 장비 스켈레톤 메시도 여기서 하는게 맞는듯
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

	UCapsuleComponent* Trigger = Cast<UCapsuleComponent>(RootComponent);

	if (Trigger != nullptr)
	{
		Trigger->OnComponentBeginOverlap.AddDynamic(this, &APHPlayableCharacter::OnItemBeginOverlap);
		Trigger->OnComponentEndOverlap.AddDynamic(this, &APHPlayableCharacter::OnItemEndOverlap);
	}
}


