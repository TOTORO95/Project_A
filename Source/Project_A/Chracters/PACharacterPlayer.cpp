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
#include "Net/UnrealNetwork.h"
#include "Project_A.h"

APACharacterPlayer::APACharacterPlayer()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	FString MaleMeshPath =
		TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple'");
	FString FemaleMeshPath =
		TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'");

	FString PhaseMesh = TEXT("/Script/Engine.SkeletalMesh'/Game/PA/Characters/Meshes/PA_Phase_GDC.PA_Phase_GDC'");
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(*PhaseMesh);
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
	///Script/Engine.AnimBlueprint'/Game/PA/Characters/Animations/ABP_Manny.ABP_Manny'
	FString PhaseAimInstancePath = TEXT("/Game/PA/Characters/Animations/ABP_Manny.ABP_Manny_C");
	FString MaleAnimInstancePath = TEXT("/Game/Characters/Mannequins/Animations/ABP_Manny.ABP_Manny_C");
	FString FemaleAnimInstancePath = TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C");
	/// Script/Engine.AnimBlueprint'/Game/PA/Characters/Blueprints/BP_PhaseAPose.BP_PhaseAPose'
	/// Script/Engine.Blueprint'/Game/PA/Characters/Blueprints/BP_ThirdPersonPhase.BP_ThirdPersonPhase'

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(*PhaseAimInstancePath);
	if (AnimInstanceRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	ThirdPersonCameraSetting();
	SetInputAction();
}

void APACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	SetupCameraType();
}

void APACharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APACharacterPlayer::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APACharacterPlayer::Look);
	}
}

void APACharacterPlayer::ThirdPersonCameraSetting()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void APACharacterPlayer::FirstPersonCameraSetting()
{
}

void APACharacterPlayer::SetInputAction()
{
	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/PA/Input/IMC_Default.IMC_Default'"));
	if (InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/PA/Input/Actions/IA_Move.IA_Move'"));
	if (InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/PA/Input/Actions/IA_Jump.IA_Jump'"));
	if (InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/PA/Input/Actions/IA_Look.IA_Look'"));
	if (InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}
}

void APACharacterPlayer::Move(const FInputActionValue& InPutValue)
{
	FVector2D MovementVector = InPutValue.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void APACharacterPlayer::Look(const FInputActionValue& InPutValue)
{
	FVector2D LookAxisVector = InPutValue.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APACharacterPlayer::SetupCameraType()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}
