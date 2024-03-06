// Fill out your copyright notice in the Description page of Project Settings.

#include "Chracters/PACharacterPlayer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APACharacterPlayer::APACharacterPlayer()
{
	ThirdPersonCameraSetting();

	//Input
	//static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT(""));
	//if (InputMappingContextRef.Object)
	//{
	//	DefaultMappingContext = InputMappingContextRef.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT(""));
	//if (InputActionMoveRef.Object)
	//{
	//	MoveAction = InputActionMoveRef.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT(""));
	//if (InputActionJumpRef.Object)
	//{
	//	JumpAction = InputActionJumpRef.Object;
	//}
}

void APACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void APACharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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


void APACharacterPlayer::Move(const FInputActionValue& InPutValue)
{
}

void APACharacterPlayer::ChangeCameraView(const FInputActionValue& InPutValue)
{
}


