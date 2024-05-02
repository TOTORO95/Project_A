// Fill out your copyright notice in the Description page of Project Settings.

#include "Chracters/PAPlayerRMCharacter.h"

#include "Animation/AnimInstance.h"
#include "Animations/PARootMotionAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Chracters/InputActionDataAsset.h"
#include "Chracters/MontageDataAsset.h"
#include "Components/CapsuleComponent.h"
#include "Engine.h"
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

APAPlayerRMCharacter::APAPlayerRMCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	SetupCamera();
	SetupCollisionBox();
	// SetupSound();

	SetMontageTable();

	// set animation blending on by default
	SetAnimationBlended(false);
	LineTraceType = ELineTraceType::CAMERA_SINGLE;
	LineTraceDistance = 0.f;
	CurrentAttackStrength = EAttackStrength::MEDIUM;
	CurrentCombo = 0;
	MaxCountdownToIdle = 30;

	FString IKDefaultAnimInstancePath = TEXT("/Script/Engine.AnimBlueprint'/Game/PA/Characters/General/ABP_PAGenAnimRM.ABP_PAGenAnimRM_C'");
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(*IKDefaultAnimInstancePath);
	if (AnimInstanceRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	SetInputAction();
	CurrentCharacterControlType = ECharacterControlType::Third;
	SetupKatana();
	CurrentAttackIndex = 0;
	bIsNextAttackInput = false;
}

void APAPlayerRMCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterControl(CurrentCharacterControlType);

	// LeftMeleeCollisionBox->OnComponentHit.AddDynamic(this, &APAPlayerRMCharacter::OnAttackHit);
	// RightMeleeCollisionBox->OnComponentHit.AddDynamic(this, &APAPlayerRMCharacter::OnAttackHit);

	// make sure our audio variables are initialized
	if (PunchSoundCue && PunchAudioComponent)
	{
		// attach the sound cue to our audio component
		// NOTE: do not do this in the constructor as it will play the sound when the player spawns
		PunchAudioComponent->SetSound(PunchSoundCue);
	}

	// make sure our audio variables are initialized
	if (PunchThrowSoundCue && PunchThrowAudioComponent)
	{
		// attach the sound cue to our audio component
		// NOTE: do not do this in the constructor as it will play the sound when the player spawns
		PunchThrowAudioComponent->SetSound(PunchThrowSoundCue);
	}
}

void APAPlayerRMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Current Attack Index = %d"), CurrentAttackIndx));

	// FireLineTrace();
}

void APAPlayerRMCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(SpaceAction, ETriggerEvent::Started, this, &APAPlayerRMCharacter::StartSpaceAction);
		EnhancedInputComponent->BindAction(SpaceAction, ETriggerEvent::Completed, this, &APAPlayerRMCharacter::CompleteSpaceAciton);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APAPlayerRMCharacter::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APAPlayerRMCharacter::SprintEnd);

		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Started, this, &APAPlayerRMCharacter::Parry);

		EnhancedInputComponent->BindAction(GuardAction, ETriggerEvent::Started, this, &APAPlayerRMCharacter::Guard);
		EnhancedInputComponent->BindAction(GuardAction, ETriggerEvent::Completed, this, &APAPlayerRMCharacter::EndGuard);

		EnhancedInputComponent->BindAction(ChangeControlAciton, ETriggerEvent::Completed, this, &APAPlayerRMCharacter::ChangeCharacterControl);
		EnhancedInputComponent->BindAction(ThirdMoveAction, ETriggerEvent::Started, this, &APAPlayerRMCharacter::MoveStart);
		EnhancedInputComponent->BindAction(ThirdMoveAction, ETriggerEvent::Triggered, this, &APAPlayerRMCharacter::ThirdMove);
		EnhancedInputComponent->BindAction(ThirdMoveAction, ETriggerEvent::Triggered, this, &APAPlayerRMCharacter::Move);

		EnhancedInputComponent->BindAction(ThirdMoveAction, ETriggerEvent::Completed, this, &APAPlayerRMCharacter::MoveEnd);

		EnhancedInputComponent->BindAction(ThirdLookAction, ETriggerEvent::Triggered, this, &APAPlayerRMCharacter::ThirdLook);

		EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &APAPlayerRMCharacter::QuaterMove);
		EnhancedInputComponent->BindAction(ChangeCombatModeAciton, ETriggerEvent::Started, this, &APAPlayerRMCharacter::ChangeCombatMode);

		EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Started, this, &APAPlayerRMCharacter::InputAttack);
		// EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &APAPlayerRMCharacter::HeavyAttack);
	}

	//// handle touch devices
	// PlayerInputComponent->BindTouch(IE_Pressed, this, &APAPlayerRMCharacter::TouchStarted);
	// PlayerInputComponent->BindTouch(IE_Released, this, &APAPlayerRMCharacter::TouchStopped);

	//// attack functionality
	// PlayerInputComponent->BindAction("Punch", IE_Pressed, this, &APAPlayerRMCharacter::PunchAttack);
	// PlayerInputComponent->BindAction("Kick", IE_Pressed, this, &APAPlayerRMCharacter::KickAttack);

	// PlayerInputComponent->BindAction("LightAttackModifier", IE_Pressed, this, &APAPlayerRMCharacter::LightAttackStart);
	// PlayerInputComponent->BindAction("LightAttackModifier", IE_Released, this, &APAPlayerRMCharacter::LightAttackEnd);

	// PlayerInputComponent->BindAction("StrongAttackModifier", IE_Pressed, this, &APAPlayerRMCharacter::StrongAttackStart);
	// PlayerInputComponent->BindAction("StrongAttackModifier", IE_Released, this, &APAPlayerRMCharacter::StrongAttackEnd);

	// PlayerInputComponent->BindAction("FireTrace", IE_Pressed, this, &APAPlayerRMCharacter::FireLineTrace);
}

void APAPlayerRMCharacter::SetupCamera()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<UPACharacterCtrlDataAsset> ThirdCtrlDataRef(TEXT("/Game/PA/CharacterControl/APA_ThirdCameraData.APA_ThirdCameraData"));
	if (ThirdCtrlDataRef.Object)
	{
		CharacterControlMap.Add(ECharacterControlType::Third, ThirdCtrlDataRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UPACharacterCtrlDataAsset> QuaterCtrlDataRef(TEXT("/Game/PA/CharacterControl/APA_QuaterCameraData.APA_QuaterCameraData"));
	if (QuaterCtrlDataRef.Object)
	{
		CharacterControlMap.Add(ECharacterControlType::Quater, QuaterCtrlDataRef.Object);
	}
}

void APAPlayerRMCharacter::SetupCollisionBox()
{
	// LeftMeleeCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftMeleeCollisionBox"));
	// LeftMeleeCollisionBox->SetupAttachment(RootComponent);
	// LeftMeleeCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	// LeftMeleeCollisionBox->SetNotifyRigidBodyCollision(false);
	// LeftMeleeCollisionBox->SetHiddenInGame(false);

	// RightMeleeCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightMeleeCollisionBox"));
	// RightMeleeCollisionBox->SetupAttachment(RootComponent);
	// RightMeleeCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	// RightMeleeCollisionBox->SetNotifyRigidBodyCollision(false);
	// RightMeleeCollisionBox->SetHiddenInGame(false);
}

void APAPlayerRMCharacter::SetupSound()
{
	// load the sound cue object
	static ConstructorHelpers::FObjectFinder<USoundCue> PunchSoundCueObject(TEXT("SoundCue'/Game/TUTORIAL_RESOURCES/Audio/PunchSoundCue.PunchSoundCue'"));
	if (PunchSoundCueObject.Succeeded())
	{
		PunchSoundCue = PunchSoundCueObject.Object;

		PunchAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PunchAudioComponent"));
		PunchAudioComponent->SetupAttachment(RootComponent);
	}

	// load the punch throw sound cue object
	static ConstructorHelpers::FObjectFinder<USoundCue> PunchThrowSoundCueObject(TEXT("SoundCue'/Game/TUTORIAL_RESOURCES/Audio/PunchThrowSoundCue.PunchThrowSoundCue'"));
	if (PunchThrowSoundCueObject.Succeeded())
	{
		PunchThrowSoundCue = PunchThrowSoundCueObject.Object;

		PunchThrowAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PunchThrowAudioComponent"));
		PunchThrowAudioComponent->SetupAttachment(RootComponent);
	}
}

bool APAPlayerRMCharacter::IsAnimationBlended()
{
	return bIsAnimationBlended;
}

void APAPlayerRMCharacter::SetAnimationBlended(bool InIsAnimationBlended)
{
	bIsAnimationBlended = InIsAnimationBlended;
}

bool APAPlayerRMCharacter::IsEquipWeapon()
{
	return bIsEquipWeapon;
}

bool APAPlayerRMCharacter::IsCombatMode()
{
	return bIsCombatMode;
}

bool APAPlayerRMCharacter::IsSprint()
{
	return bIsSprint;
}

bool APAPlayerRMCharacter::IsDodge()
{
	return bIsDodge;
}

void APAPlayerRMCharacter::SetNextAttackPossible(bool InNextAttackPossible)
{
	bIsNextAttackPossible = InNextAttackPossible;
}

bool APAPlayerRMCharacter::IsNextAttackPossible()
{
	return bIsNextAttackPossible;
}

bool APAPlayerRMCharacter::IsNextAttackInput()
{
	return bIsNextAttackInput;
}

void APAPlayerRMCharacter::SetNextAttackInput(bool InNextAttackInput)
{
	bIsNextAttackInput = InNextAttackInput;
}

void APAPlayerRMCharacter::SetIsKeyboardEnabled(bool Enabled)
{
	bIsKeyboardEnabled = Enabled;
}

EAttackType APAPlayerRMCharacter::GetCurrentAttack()
{
	return CurrentAttack;
}

EWeaponType APAPlayerRMCharacter::GetCurrentWeaponType()
{
	return CurrentWeaponType;
}

void APAPlayerRMCharacter::ResetCombo()
{
}

void APAPlayerRMCharacter::EquipWeaponStart()
{
	EquipWeapon();
}

void APAPlayerRMCharacter::EquipWeapon()
{
}

void APAPlayerRMCharacter::EquipWeaponEnd()
{
}

void APAPlayerRMCharacter::CrouchStart()
{
	Crouch();
}

void APAPlayerRMCharacter::CrouchEnd()
{
}

void APAPlayerRMCharacter::SetupKatana()
{
	KatanaSwordSocket = CreateDefaultSubobject<UStaticMeshComponent>("KatanaSword");
	if (GetMesh()->DoesSocketExist("Katana_weapon_01"))
	{
		KatanaSwordSocket->SetupAttachment(GetMesh(), "Katana_weapon_01");
	}

	KatanaSheathSocket = CreateDefaultSubobject<UStaticMeshComponent>("KatanaSheathSocket");
	if (GetMesh()->DoesSocketExist("Katana_sheath_01"))
	{
		KatanaSheathSocket->SetupAttachment(GetMesh(), "Katana_sheath_01");
	}

	KatanaSheathInSocket = CreateDefaultSubobject<UStaticMeshComponent>("KatanaSheathInSocket");
	if (GetMesh()->DoesSocketExist("Katana_sheath_02"))
	{
		KatanaSheathInSocket->SetupAttachment(GetMesh(), "Katana_sheath_02");
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> KatanaSwordMesh = (TEXT("/Game/PA/Item/Weapon/Katana_sword.Katana_sword"));
	if (KatanaSwordMesh.Succeeded())
	{
		KatanaSheathInSocket->SetStaticMesh(KatanaSwordMesh.Object);
		KatanaSwordSocket->SetStaticMesh(KatanaSwordMesh.Object);
		KatanaSwordSocket->SetVisibility(false);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> KatanaSheathMesh = (TEXT("/Game/PA/Item/Weapon/Katana_sheath.Katana_sheath"));
	if (KatanaSheathMesh.Succeeded())
	{
		KatanaSheathSocket->SetStaticMesh(KatanaSheathMesh.Object);
	}
}

void APAPlayerRMCharacter::SetCharacterControl(ECharacterControlType InCharacterControlType)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	UPACharacterCtrlDataAsset* CharacterControlData = CharacterControlMap[InCharacterControlType];
	if (!CharacterControlData)
	{
		return;
	}

	SetCharacterCtrlData(CharacterControlData);
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* MappingContext = CharacterControlData->InputMappingContext;
		if (MappingContext)
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
	CurrentCharacterControlType = InCharacterControlType;
}

void APAPlayerRMCharacter::SetCharacterCtrlData(const UPACharacterCtrlDataAsset* InCharacterCtrlDataAsset)
{
	// Pawn
	bUseControllerRotationYaw = InCharacterCtrlDataAsset->buseControllerRotationYaw;

	// ChacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = InCharacterCtrlDataAsset->bOientRotationToMovement;
	GetCharacterMovement()->RotationRate = InCharacterCtrlDataAsset->RotationRate;
	GetCharacterMovement()->bUseControllerDesiredRotation = InCharacterCtrlDataAsset->bUseControllerDesiredRotation;
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;

	// Camera
	CameraBoom->TargetArmLength = InCharacterCtrlDataAsset->TargetArmLength;
	CameraBoom->SetRelativeRotation(InCharacterCtrlDataAsset->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = InCharacterCtrlDataAsset->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = InCharacterCtrlDataAsset->bInheritPitch;
	CameraBoom->bInheritRoll = InCharacterCtrlDataAsset->bInheritRoll;
	CameraBoom->bInheritYaw = InCharacterCtrlDataAsset->bInheritYaw;
	CameraBoom->bDoCollisionTest = InCharacterCtrlDataAsset->bDoCollisionTest;
}

void APAPlayerRMCharacter::ChangeCharacterControl()
{
	switch (CurrentCharacterControlType)
	{
		case ECharacterControlType::Third:
			SetCharacterControl(ECharacterControlType::Quater);
			break;
		case ECharacterControlType::Quater:
			SetCharacterControl(ECharacterControlType::Third);
			break;
		default:
			SetCharacterControl(ECharacterControlType::Third);
			break;
	}
}

void APAPlayerRMCharacter::SetInputAction()
{
	static ConstructorHelpers::FObjectFinder<UInputActionDataAsset> InputActionData(TEXT("/Game/PA/Input/Actions/InputActionDataAsset.InputActionDataAsset"));
	if (!InputActionData.Object)
	{
		return;
	}

	auto InputDatas = InputActionData.Object->InputActions;
	if (!InputDatas.Num())
	{
		return;
	}
	for (int i = 0; i < InputDatas.Num(); i++)
	{
		FString ActionName = InputDatas[i]->GetName().RightChop(3);
		PA_LOG(LogTemp, Log, TEXT("Input Action Name = %s  Path = %s"), *ActionName, *InputDatas[i]->GetPathName());
		InputActionMap.Add(*ActionName, InputDatas[i]);
	}

	auto CheckFindFunc = [&](TObjectPtr<class UInputAction>& InputAction, FString ActionName)
	{
		TObjectPtr<UInputAction>* InputActionAsset = InputActionMap.Find(ActionName);
		if (InputActionAsset)
		{
			InputAction = *InputActionAsset;
		}
	};
	CheckFindFunc(JumpAction, TEXT("Jump"));
	CheckFindFunc(ThirdMoveAction, TEXT("ThirdMove"));
	CheckFindFunc(ThirdLookAction, TEXT("ThirdLook"));
	CheckFindFunc(QuaterMoveAction, TEXT("QuaterMove"));
	CheckFindFunc(ChangeCombatModeAciton, TEXT("ChangeCombatMode"));
	CheckFindFunc(SprintAction, TEXT("Sprint"));
	CheckFindFunc(LightAttackAction, TEXT("LightAttack"));
	CheckFindFunc(DodgeAction, TEXT("Dodge"));
	CheckFindFunc(ParryAction, TEXT("Parry"));
	CheckFindFunc(GuardAction, TEXT("Guard"));
	CheckFindFunc(SpaceAction, TEXT("Space"));

	// CheckFindFunc(HeavyAttackAction, TEXT("HeavyAttack"));
}

void APAPlayerRMCharacter::SetMontageTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PlayerAttackMontageDataTableObject(TEXT("DataTable'/Game/PA/DataTables/Samuri_CBADataTable.Samuri_CBADataTable'"));
	if (PlayerAttackMontageDataTableObject.Succeeded())
	{
		PlayerAttackDataTable = PlayerAttackMontageDataTableObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> MovementDataTableObject(TEXT("DataTable'/Game/PA/DataTables/MovementMontageTable.MovementMontageTable'"));
	if (MovementDataTableObject.Succeeded())
	{
		MovementDataTable = MovementDataTableObject.Object;
		if (!MovementDataTable->GetRowMap().IsEmpty())
		{
			MovementMontageArray.Add(MovementDataTable->FindRow<FMovementMontage>(FName("Walk"), TEXT(""), true)->Montage);
			MovementMontageArray.Add(MovementDataTable->FindRow<FMovementMontage>(FName("Sprint"), TEXT(""), true)->Montage);
			MovementMontageArray.Add(MovementDataTable->FindRow<FMovementMontage>(FName("Walk_Combat"), TEXT(""), true)->Montage);
			MovementMontageArray.Add(MovementDataTable->FindRow<FMovementMontage>(FName("Sprint_Combat"), TEXT(""), true)->Montage);
		}
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> AvoidDataTableObject(TEXT("DataTable'/Game/PA/DataTables/Samuri_SpaceActionMontage.Samuri_SpaceActionMontage'"));
	if (AvoidDataTableObject.Succeeded())
	{
		AvoidDataTable = AvoidDataTableObject.Object;
		if (!AvoidDataTable->GetRowMap().IsEmpty())
		{
			AvoidMontageArray.Add(AvoidDataTable->FindRow<FAvoidMontage>(FName("Jump"), TEXT(""), true)->Montage);
			AvoidMontageArray.Add(AvoidDataTable->FindRow<FAvoidMontage>(FName("Dodge"), TEXT(""), true)->Montage);
		}
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> DefenceDataTableObject(TEXT("DataTable'/Game/PA/DataTables/Samuri_DefenceMontageTable.Samuri_DefenceMontageTable'"));
	if (DefenceDataTableObject.Succeeded())
	{
		DefenceDataTable = DefenceDataTableObject.Object;
		if (!DefenceDataTable->GetRowMap().IsEmpty())
		{
			DefenceMontageArray.Add(DefenceDataTable->FindRow<FMontageData>(FName("Defence"), TEXT(""), true)->Montage);
			DefenceMontageArray.Add(DefenceDataTable->FindRow<FMontageData>(FName("Parry"), TEXT(""), true)->Montage);
		}
	}
}

void APAPlayerRMCharacter::StartSpaceAction()
{
	if (!bIsCombatMode)
	{
		Jump();
	}
	else
	{
		Dodge();
	}
}

void APAPlayerRMCharacter::CompleteSpaceAciton()
{
	if (!bIsCombatMode)
	{
		StopJumping();
	}
}

void APAPlayerRMCharacter::ChangeCombatMode(const FInputActionValue& InPutValue)
{
	PA_LOG(LogTemp, Log, TEXT("In"));
}

void APAPlayerRMCharacter::ThirdMove(const FInputActionValue& InPutValue)
{
	FVector2D MovementVector = InPutValue.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Debug
	float LineLength = 50.0f;
	FVector PlayerPosition = GetCharacterMovement()->GetActorLocation();

	FVector ForwardLine = PlayerPosition + (ForwardDirection * LineLength);
	FVector RightLine = PlayerPosition + (RightDirection * LineLength);

	// DrawDebugLine(GetWorld(), PlayerPosition, RightLine, FColor(0, 0, 255), false, 2.0f, 0, 3.0f);
	// DrawDebugLine(GetWorld(), PlayerPosition, ForwardLine, FColor(255, 0, 0), false, 2.0f, 0, 3.0f);

	// PA_LOG(LogTemp, Log, TEXT("MovementVector X= %f /  Y = %f"), MovementVector.X, MovementVector.Y);
	// PA_LOG(LogTemp, Log, TEXT("ForwardDirection = %f %f %f"), ForwardDirection.X, ForwardDirection.Y, ForwardDirection.Z);

	FVector ForwardDirectionScale = ForwardDirection * MovementVector.X;
	FVector RightDirectionScale = RightDirection * MovementVector.Y;

	FVector DirVec = ForwardDirectionScale + RightDirectionScale;

	DirVec.Normalize();
	FVector ActorForwardVector = GetActorForwardVector();
	ActorForwardVector.Normalize();
	float dot = FVector::DotProduct(DirVec, ActorForwardVector);
	FVector CrossProdot = FVector::CrossProduct(ActorForwardVector, DirVec);
	float angle = FMath::RadiansToDegrees(FMath::Acos(dot));
	if (CrossProdot.Z < 0.0f)
	{
		angle *= -1.0f;
	}

	// PA_LOG(LogTemp, Log, TEXT("InnerAngle = %f "), angle);
	AddActorLocalRotation(FRotator(0.0f, angle * 0.5f, 0.0f));

	// AddActorLocalRotation();
	// SetActorLocationAndRotation(GetActorLocation(), DirVec.ToOrientationRotator());
}

void APAPlayerRMCharacter::ThirdLook(const FInputActionValue& InPutValue)
{
	FVector2D LookAxisVector = InPutValue.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APAPlayerRMCharacter::QuaterMove(const FInputActionValue& InPutValue)
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

void APAPlayerRMCharacter::InputAttack(const FInputActionValue& InPutValue)
{
	bool bIsLightAttackInput = InPutValue.Get<bool>();
	bUseControllerRotationYaw = false;	  // Animation에서 제어할예정
	AttackMontage = PlayerAttackDataTable->FindRow<FPlayerAttackMontage>(AttackRowKey, "5", true);
	SetAnimationBlended(false);
	UAnimInstance* AnimInstance = GetAnimInstance();
	if (!AnimInstance->Montage_IsPlaying(AttackMontage->Montage))
	{
		CurrentAttackIndex = 0;
	}

	if (CurrentAttackIndex == 0)
	{
		BeginComboAttack();
	}
	else
	{
		SetNextAttackInput(IsNextAttackPossible());
	}
}

void APAPlayerRMCharacter::BeginComboAttack()
{
	SetNextAttackInput(false);
	FString InitAttackSection = FString::Printf(TEXT("~%02d_Start"), ++CurrentAttackIndex);
	PlayAnimMontage(AttackMontage->Montage, 1.f, *InitAttackSection);
}

void APAPlayerRMCharacter::NextComboAttack()
{
	SetNextAttackPossible(false);
	SetNextAttackInput(false);
	CurrentAttackIndex = FMath::Clamp(CurrentAttackIndex + 1, 0, AttackMontage->AnimSectionCount);
	FString NextAttackIndex = FString::Printf(TEXT("LightAttack%02d_Start"), CurrentAttackIndex);
	GetAnimInstance()->Montage_Stop(0.1f, AttackMontage->Montage);
	PlayAnimMontage(AttackMontage->Montage, 1.f, *NextAttackIndex);
}

void APAPlayerRMCharacter::SprintStart(const FInputActionValue& InPutValue)
{
	bIsSprint = InPutValue.Get<bool>();
	uint8 CurrentMovementState = bIsCombatMode * 2 + bIsSprint;
	UAnimInstance* AnimInstance = GetAnimInstance();
	bool IsMovementMontage = false;

	for (auto MovementMontage : MovementMontageArray)
	{
		if (!MovementMontage)
		{
			return;
		}
		if (AnimInstance->Montage_IsPlaying(MovementMontage))
		{
			IsMovementMontage = true;
		}
	}
	if (!IsMovementMontage)
	{
		return;
	}

	FString MontageSection = AnimInstance->Montage_GetCurrentSection(GetCurrentMontage()).ToString();
	if (MontageSection.Find("End_") != INDEX_NONE)
	{
		return;
	}

	if (GetCurrentMontage()->GetName().Find("Walk"))
	{
		PlayAnimMontage(MovementMontageArray[CurrentMovementState], 1.f, FName(CurrentMoveMontageSection));
	}
}

void APAPlayerRMCharacter::SprintEnd(const FInputActionValue& InPutValue)
{
	bIsSprint = InPutValue.Get<bool>();
	uint8 CurrentMovementState = bIsCombatMode * 2 + bIsSprint;
	UAnimInstance* AnimInstance = GetAnimInstance();
	bool IsMovementMontage = false;
	for (auto MovementMontage : MovementMontageArray)

	{
		if (!MovementMontage)
		{
			return;
		}
		if (AnimInstance->Montage_IsPlaying(MovementMontage))
		{
			IsMovementMontage = true;
		}
	}

	if (!IsMovementMontage)
	{
		return;
	}
	FString MontageSection = AnimInstance->Montage_GetCurrentSection(GetCurrentMontage()).ToString();
	if (MontageSection.Find("End_") != INDEX_NONE)
	{
		return;
	}

	if (GetCurrentMontage()->GetName().Find("Sprint"))
	{
		PlayAnimMontage(MovementMontageArray[CurrentMovementState], 1.f, FName(CurrentMoveMontageSection));
	}
}

void APAPlayerRMCharacter::MoveStart()
{
	uint8 CurrentMovementState = bIsCombatMode * 2 + bIsSprint;
	UAnimInstance* AnimInstance = GetAnimInstance();

	CurrentMoveMontageSection = TEXT("Start_Start");
	PA_LOG(LogTemp, Log, TEXT("Current Playing Montage State = %d"), CurrentMovementState);
	if (FName(CurrentMoveMontageSection) == AnimInstance->Montage_GetCurrentSection(MovementMontageArray[CurrentMovementState]))
	{
		PA_LOG(LogTemp, Log, TEXT("이미 Start Anim 재생 중....."));
		return;
	}
	PlayAnimMontage(MovementMontageArray[CurrentMovementState], 1.f, FName(*CurrentMoveMontageSection));
}

void APAPlayerRMCharacter::Dodge()
{
	PA_LOG(LogTemp, Log, TEXT("Dodge Start !"));

	if (GetAnimInstance()->Montage_IsPlaying(AvoidMontageArray[1]))
	{
		return;
	}
	GetAnimInstance()->StopAllMontages(0.1f);
	PlayAnimMontage(AvoidMontageArray[1], 1.0f, FName("Action_Start_Start"));
}

void APAPlayerRMCharacter::RightAction(const FInputActionValue& InPutValue)
{
	bool bIsInput = InPutValue.Get<bool>();
	if (bIsInput)
	{
		PA_LOG(LogTemp, Log, TEXT("RightAction Trigger!"));
		Guard(InPutValue);
	}
	else
	{
		PA_LOG(LogTemp, Log, TEXT("RightAction Trigger Cancel!"));
		EndGuard(InPutValue);
	}
}

void APAPlayerRMCharacter::Guard(const FInputActionValue& InPutValue)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	UAnimMontage* GuardMontage = DefenceMontageArray[0];
	FName CurrentSection = GetAnimInstance()->Montage_GetCurrentSection(GuardMontage);
	if (!GetAnimInstance()->Montage_IsPlaying(GuardMontage) /*|| CurrentSection.Compare(FName("Action_End_Start"))==0 */)
	{
		SetAnimationBlended(true);
		PlayAnimMontage(GuardMontage, 1.0f, FName("Action_Start_Start"));
	}
}

void APAPlayerRMCharacter::EndGuard(const FInputActionValue& InPutValue)
{
	PA_LOG(LogTemp, Log, TEXT("RightAction Trigger Cancel!"));
	SetAnimationBlended(true);

	UAnimMontage* GuardMontage = DefenceMontageArray[0];
	FName CurrentSection = GetAnimInstance()->Montage_GetCurrentSection(GuardMontage);
	StopAnimMontage(GuardMontage);
	PlayAnimMontage(GuardMontage, 1.0f, "Action_End_Start");
}

void APAPlayerRMCharacter::Parry(const FInputActionValue& InPutValue)
{
	bool bIsParry = InPutValue.Get<bool>();
	if (bIsParry)
	{
		PA_LOG(LogTemp, Log, TEXT("Parry Start !"));
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		SetAnimationBlended(true);

		UAnimMontage* ParryMontage = DefenceMontageArray[1];
		if (!GetAnimInstance()->Montage_IsPlaying(ParryMontage))
		{
			PlayAnimMontage(ParryMontage, 1.0f, FName("Action_Start_Start"));
		}
	}
}

void APAPlayerRMCharacter::ParryAttack()
{
	SetAnimationBlended(true);

	UAnimMontage* ParryMontage = DefenceMontageArray[1];
	if (GetAnimInstance()->Montage_IsPlaying(ParryMontage))
	{
		PA_LOG(LogTemp, Log, TEXT("EndParry Start !"));
		StopAnimMontage(ParryMontage);
		PlayAnimMontage(ParryMontage, 0.5f, "Action_End_Start");
	}
}

void APAPlayerRMCharacter::Move()
{
	FName ReadySection = TEXT("Start_Start");
	uint8 CurrentMovementState = bIsCombatMode * 2 + bIsSprint;
	// PA_LOG(LogTemp, Log, TEXT("Current Playing Montage State = %d"), CurrentMovementState);

	FName CurrentPlaySection = GetAnimInstance()->Montage_GetCurrentSection(MovementMontageArray[CurrentMovementState]);
	if (ReadySection == CurrentPlaySection)
	{
		// PA_LOG(LogTemp, Log, TEXT("Ready Move 재생 중....."));
		return;
	}

	CurrentMoveMontageSection = TEXT("Loop_Start");
	if (GetAnimInstance()->Montage_IsPlaying(MovementMontageArray[CurrentMovementState]) && FName(CurrentMoveMontageSection) == CurrentPlaySection)
	{
		// PA_LOG(LogTemp, Log, TEXT("이미 Start Anim 재생 중....."));
		return;
	}
	PlayAnimMontage(MovementMontageArray[CurrentMovementState], 1.f, FName(*CurrentMoveMontageSection));
}

void APAPlayerRMCharacter::MoveEnd()
{
	FName ReadySection = TEXT("End_Start");
	UAnimMontage* CrurrentMovementMontage = nullptr;
	FName CurrentPlaySection;
	for (auto MovementMontage : MovementMontageArray)
	{
		if (GetAnimInstance()->Montage_IsPlaying(MovementMontage))
		{
			CrurrentMovementMontage = MovementMontage;
			CurrentPlaySection = GetAnimInstance()->Montage_GetCurrentSection(MovementMontage);
		}
	}

	CurrentMoveMontageSection = "End_Start";
	PlayAnimMontage(CrurrentMovementMontage, 1.f, FName(*CurrentMoveMontageSection));
	// UAnimMontage* CurrentMonatage = GetAnimInstance()->GetCurrentActiveMontage();
	// FString PrevMontageSection = GetAnimInstance()->Montage_GetCurrentSection();
	// if (CurrentMoveMontageSection == PrevMontageSection)
	//{
	//	return;
	//}

	// GetAnimInstance()->Montage_Stop(0.5f, CurrentMonatage);
	// PlayAnimMontage(CrurrentMovementMontage, 1.f, FName(*CurrentMoveMontageSection));
}

void APAPlayerRMCharacter::MountingWeapon()
{
	bIsCombatMode = true;
	KatanaSwordSocket->SetVisibility(true);
	KatanaSheathInSocket->SetVisibility(false);
}

void APAPlayerRMCharacter::ReleaseWeapon()
{
	bIsCombatMode = false;
	CurrentAttackIndex = 0;
	KatanaSwordSocket->SetVisibility(false);
	KatanaSheathInSocket->SetVisibility(true);
}

void APAPlayerRMCharacter::OnAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}
