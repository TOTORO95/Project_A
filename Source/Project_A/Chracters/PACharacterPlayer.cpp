// Fill out your copyright notice in the Description page of Project Settings.

#include "Chracters/PACharacterPlayer.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Items/PAWeaponPrimaryDataAsset.h"
#include "Net/UnrealNetwork.h"
#include "PACharacterCtrlDataAsset.h"
#include "Project_A.h"

APACharacterPlayer::APACharacterPlayer()
{
	SetupCamera();
	SetupControlDataAsset();

	SetInputAction();
	CurrentCharacterCtrlype = ECharacterCtrlType::Quater;
}

void APACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterCtrl(CurrentCharacterCtrlype);
}

void APACharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APACharacterPlayer::Sprint);
		EnhancedInputComponent->BindAction(ChangeCtrlAciton, ETriggerEvent::Completed, this, &APACharacterPlayer::ChangeCharacterControl);
		EnhancedInputComponent->BindAction(ThirdMoveAction, ETriggerEvent::Triggered, this, &APACharacterPlayer::ThirdMove);
		EnhancedInputComponent->BindAction(ThirdLookAction, ETriggerEvent::Triggered, this, &APACharacterPlayer::ThirdLook);
		EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &APACharacterPlayer::QuaterMove);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APACharacterPlayer::Attack);
	}
}

void APACharacterPlayer::SetupControlDataAsset()
{
	static ConstructorHelpers::FObjectFinder<UPACharacterCtrlDataAsset> ThirdCtrlDataRef(TEXT("/Game/PA/CharacterControl/APA_ThirdCameraData.APA_ThirdCameraData"));
	if (ThirdCtrlDataRef.Object)
	{
		CharacterCtrlMap.Add(ECharacterCtrlType::Third, ThirdCtrlDataRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UPACharacterCtrlDataAsset> QuaterCtrlDataRef(TEXT("/Game/PA/CharacterControl/APA_QuaterCameraData.APA_QuaterCameraData"));

	if (QuaterCtrlDataRef.Object)
	{
		CharacterCtrlMap.Add(ECharacterCtrlType::Quater, QuaterCtrlDataRef.Object);
	}
}

void APACharacterPlayer::SetupCamera()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void APACharacterPlayer::ChangeCharacterControl()
{
	switch (CurrentCharacterCtrlype)
	{
		case ECharacterCtrlType::Third:
			SetCharacterCtrl(ECharacterCtrlType::Quater);
			break;
		case ECharacterCtrlType::Quater:
			SetCharacterCtrl(ECharacterCtrlType::Third);
			break;
		default:
			SetCharacterCtrl(ECharacterCtrlType::Third);
			break;
	}
}

void APACharacterPlayer::SetCharacterCtrl(ECharacterCtrlType InCharacterControlType)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	UPACharacterCtrlDataAsset* CharacterCtrl = CharacterCtrlMap[InCharacterControlType];
	if (!CharacterCtrl)
	{
		return;
	}

	SetCharacterCtrlData(CharacterCtrl);
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* MappingContext = CharacterCtrl->InputMappingContext;
		if (MappingContext)
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
	CurrentCharacterCtrlype = InCharacterControlType;
}

void APACharacterPlayer::SetCharacterCtrlData(const UPACharacterCtrlDataAsset* InCharacterCtrlDataAsset)
{
	// Pawn
	bUseControllerRotationYaw = InCharacterCtrlDataAsset->buseControllerRotationYaw;

	// ChacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = InCharacterCtrlDataAsset->bOientRotationToMovement;
	GetCharacterMovement()->RotationRate = InCharacterCtrlDataAsset->RotationRate;
	GetCharacterMovement()->bUseControllerDesiredRotation = InCharacterCtrlDataAsset->bUseControllerDesiredRotation;

	// Camera
	CameraBoom->TargetArmLength = InCharacterCtrlDataAsset->TargetArmLength;
	CameraBoom->SetRelativeRotation(InCharacterCtrlDataAsset->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = InCharacterCtrlDataAsset->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = InCharacterCtrlDataAsset->bInheritPitch;
	CameraBoom->bInheritRoll = InCharacterCtrlDataAsset->bInheritRoll;
	CameraBoom->bInheritYaw = InCharacterCtrlDataAsset->bInheritYaw;
	CameraBoom->bDoCollisionTest = InCharacterCtrlDataAsset->bDoCollisionTest;
}

void APACharacterPlayer::SetInputAction()
{
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PA/Input/Actions/IA_Jump.IA_Jump'"));
	if (InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionThirdMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PA/Input/Actions/IA_ThirdMove.IA_ThirdMove'"));
	if (InputActionThirdMoveRef.Object)
	{
		ThirdMoveAction = InputActionThirdMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionThirdLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PA/Input/Actions/IA_ThirdLook.IA_ThirdLook'"));
	if (InputActionThirdLookRef.Object)
	{
		ThirdLookAction = InputActionThirdLookRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionChangeCtrlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PA/Input/Actions/IA_ChangeCtrlType.IA_ChangeCtrlType'"));
	if (InputActionChangeCtrlRef.Object)
	{
		ChangeCtrlAciton = InputActionChangeCtrlRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PA/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionChudanAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PA/Input/Actions/IA_ChudanAttack.IA_ChudanAttack'"));
	if (InputActionChudanAttackRef.Object)
	{
		AttackAction = InputActionChudanAttackRef.Object;
	}
}

void APACharacterPlayer::Sprint(const FInputActionValue& InPutValue)
{
	
}

void APACharacterPlayer::ThirdMove(const FInputActionValue& InPutValue)
{
	FVector2D MovementVector = InPutValue.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void APACharacterPlayer::ThirdLook(const FInputActionValue& InPutValue)
{
	FVector2D LookAxisVector = InPutValue.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APACharacterPlayer::QuaterMove(const FInputActionValue& InPutValue)
{
	FVector2D MovementVector = InPutValue.Get<FVector2D>();
	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSize > 1.f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}
	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void APACharacterPlayer::Attack(const FInputActionValue& InPutValue)
{
	ProcessComboCommand();
}
