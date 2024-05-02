// Fill out your copyright notice in the Description page of Project Settings.

#include "Chracters/PACharacterBase.h"

#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Components/IKFootActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PACharacterCtrlDataAsset.h"
#include "PAComboAttackDataAsset.h"
#include "Physics/PACollision.h"
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
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	
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
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PACAPSULE);
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

void APACharacterBase::MountingWeapon()
{
}

void APACharacterBase::ReleaseWeapon()
{
}

void APACharacterBase::AttackHitCheck()
{
	TArray<FHitResult> OutHitResults;
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 70.0f;
	const float AttackRadius = 80.0f;
	const float AttackDamage = 40.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, CCHANNEL_PAACTION, FCollisionShape::MakeSphere(AttackRange), Params);
	//bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_PAACTION, FCollisionShape::MakeSphere(AttackRange), Params);

	if (HitDetected)
	{
		for (auto HitResult : OutHitResults)
		{
			//HitResult.GetActor()->getObjectname
			PA_LOG(LogTemp, Log, TEXT("Hit %s"), *HitResult.GetActor()->GetFName().ToString());

		}
		
		//OutHitResult.GetActor()->GetName()
	}
#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Red : FColor::Green;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRange, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 1.5f);
#endif
}

void APACharacterBase::PlayNextSectionMontage(UAnimMontage* Montage, float PlayRate, FName NextSection)
{
	PlayAnimMontage(Montage, PlayRate, NextSection);
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

	GetAnimInstance()->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APACharacterBase::ComboActionEnd);
	GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

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
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionDataAsset->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionDataAsset->MontageSectionNamePrefix, CurrentCombo);
		GetAnimInstance()->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

TObjectPtr<class UAnimInstance> APACharacterBase::GetAnimInstance()
{
	return GetMesh()->GetAnimInstance();
}

