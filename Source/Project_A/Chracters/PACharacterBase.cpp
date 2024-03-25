// Fill out your copyright notice in the Description page of Project Settings.

#include "Chracters/PACharacterBase.h"

#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Components/IKFootActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PACharacterCtrlDataAsset.h"
#include "PAComboAttackDataAsset.h"
#include "Project_A.h"

// Sets default values
APACharacterBase::APACharacterBase()
{
	InitCharacterComponent();
}

// Called when the game starts or when spawned
void APACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APACharacterBase::InitCharacterComponent()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	
	FString UE5DefaultMeshPath = TEXT("/Script/Engine.SkeletalMesh'/Game/PA/Characters/Samuri/SKM_PA_Samuri.SKM_PA_Samuri'");
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(*UE5DefaultMeshPath);
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	FString IKDefaultAnimInstancePath = TEXT("/Script/Engine.AnimBlueprint'/Game/PA/Characters/General/ABP_IKGenAnim.ABP_IKGenAnim_C'");
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(*IKDefaultAnimInstancePath);
	if (AnimInstanceRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	InitControllerRotation();
	InitCollisionCompoent();
	InitMovementComponent();

	IKFootComponent = CreateDefaultSubobject<UIKFootActorComponent>(TEXT("IKFootComponent"));
	IKFootComponent->Set_IKSocketName(TEXT("foot_l"), TEXT("foot_r"));
}

void APACharacterBase::InitControllerRotation()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void APACharacterBase::InitCollisionCompoent()
{
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
}

void APACharacterBase::InitMovementComponent()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

void APACharacterBase::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void APACharacterBase::ComboActionBegin()
{
	CurrentCombo = 1;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	const float AttackSpeedRate = 1.f;	  // TODO: AttackSpeedRate Convert Character Stat

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APACharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void APACharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsPropelyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void APACharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionDataAsset->EffectiveFrameCount.IsValidIndex(ComboIndex));
	const float AttackSpeedRate = 1.f;
	float ComboEffectiveFrame = ComboActionDataAsset->EffectiveFrameCount[ComboIndex];

	float ComboFrameRate = ComboActionDataAsset->FrameRate;

	float ComboEffectiveTime = (ComboEffectiveFrame / ComboFrameRate) / AttackSpeedRate;
	PA_NETLOG(LogPANetwork, Log, TEXT("ComboEffectiveTime = %f"), ComboEffectiveTime);

	if (ComboEffectiveTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &APACharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

void APACharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionDataAsset->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionDataAsset->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}
