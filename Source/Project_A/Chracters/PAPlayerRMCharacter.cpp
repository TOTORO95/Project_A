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
	// load player attack montage data table
	// /Script/Engine.DataTable'/Game/PA/Characters/Samuri/Animations/Attack/Samuri_CBADataTable.Samuri_CBADataTable'

	// set animation blending on by default
	bIsAnimationBlended = true;
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
	CurrentAttackIndx = 0;
	bIsNextAttackInput = false;
}

void APAPlayerRMCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterControl(CurrentCharacterControlType);

	LeftMeleeCollisionBox->OnComponentHit.AddDynamic(this, &APAPlayerRMCharacter::OnAttackHit);
	RightMeleeCollisionBox->OnComponentHit.AddDynamic(this, &APAPlayerRMCharacter::OnAttackHit);

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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Current Attack Index = %d"), CurrentAttackIndx));

	// FireLineTrace();
}

void APAPlayerRMCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APAPlayerRMCharacter::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APAPlayerRMCharacter::SprintEnd);

		EnhancedInputComponent->BindAction(ChangeControlAciton, ETriggerEvent::Completed, this, &APAPlayerRMCharacter::ChangeCharacterControl);
		EnhancedInputComponent->BindAction(ThirdMoveAction, ETriggerEvent::Started, this, &APAPlayerRMCharacter::MoveStart);
		EnhancedInputComponent->BindAction(ThirdMoveAction, ETriggerEvent::Triggered, this, &APAPlayerRMCharacter::ThirdMove);

		EnhancedInputComponent->BindAction(ThirdMoveAction, ETriggerEvent::Completed, this, &APAPlayerRMCharacter::MoveEnd);

		EnhancedInputComponent->BindAction(ThirdLookAction, ETriggerEvent::Triggered, this, &APAPlayerRMCharacter::ThirdLook);

		EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &APAPlayerRMCharacter::QuaterMove);
		EnhancedInputComponent->BindAction(ChangeCombatModeAciton, ETriggerEvent::Started, this, &APAPlayerRMCharacter::ChangeCombatMode);

		EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Started, this, &APAPlayerRMCharacter::LightAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &APAPlayerRMCharacter::HeavyAttack);
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
	LeftMeleeCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftMeleeCollisionBox"));
	LeftMeleeCollisionBox->SetupAttachment(RootComponent);
	LeftMeleeCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	LeftMeleeCollisionBox->SetNotifyRigidBodyCollision(false);
	LeftMeleeCollisionBox->SetHiddenInGame(false);

	RightMeleeCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightMeleeCollisionBox"));
	RightMeleeCollisionBox->SetupAttachment(RootComponent);
	RightMeleeCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	RightMeleeCollisionBox->SetNotifyRigidBodyCollision(false);
	RightMeleeCollisionBox->SetHiddenInGame(false);
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

void APAPlayerRMCharacter::SetNextAttackPossible(bool InNextAttackPossible, uint8 inCurrentAttackIndex)
{
	bIsNextAttackPossible = InNextAttackPossible;
	// CurrentAttackIndx = inCurrentAttackIndex;
	// if (!bIsNextAttackPossible)
	//{
	//	CurrentAttackIndx = 0;
	// }
}

bool APAPlayerRMCharacter::IsNextAttackPossible()
{
	return bIsNextAttackPossible;
}

bool APAPlayerRMCharacter::GetNextAttackInput()
{
	return bIsNextAttackInput;
}

void APAPlayerRMCharacter::InitNextAttackInput()
{
	bIsNextAttackInput = false;
}

void APAPlayerRMCharacter::StartNextAttack()
{
	CurrentAttackIndx = FMath::Clamp(CurrentAttackIndx + 1, 0, AttackMontage->AnimSectionCount);
	FString NextAttackIndex = FString::Printf(TEXT("LightAttack%02d_Start"), CurrentAttackIndx);
	GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, AttackMontage->Montage);
	PlayAnimMontage(AttackMontage->Montage, 1.f, *NextAttackIndex);
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
	CheckFindFunc(HeavyAttackAction, TEXT("HeavyAttack"));
}

void APAPlayerRMCharacter::SetMontageTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PlayerAttackMontageDataTableObject(TEXT("DataTable'/Game/PA/Characters/Samuri/Animations/Attack/Samuri_CBADataTable.Samuri_CBADataTable'"));
	if (PlayerAttackMontageDataTableObject.Succeeded())
	{
		PlayerAttackDataTable = PlayerAttackMontageDataTableObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> MovementDataTableObject(TEXT("DataTable'/Game/PA/DataTables/MovementMontageTable.MovementMontageTable'"));
	if (MovementDataTableObject.Succeeded())
	{
		MovementDataTable = MovementDataTableObject.Object;

		WalkMontage = MovementDataTable->FindRow<FMovementMontage>(FName("Walk"), TEXT(""), true);
		SprintMontage = MovementDataTable->FindRow<FMovementMontage>(FName("Sprint"), TEXT(""), true);
		CombatWalkMontage = MovementDataTable->FindRow<FMovementMontage>(FName("Walk_Combat"), TEXT(""), true);
		CombatSprintMontage = MovementDataTable->FindRow<FMovementMontage>(FName("Sprint_Combat"), TEXT(""), true);

		if (WalkMontage && SprintMontage && CombatWalkMontage && CombatSprintMontage)
		{
			MovementMontageArray.Add(WalkMontage->Montage);
			MovementMontageArray.Add(SprintMontage->Montage);
			MovementMontageArray.Add(CombatWalkMontage->Montage);
			MovementMontageArray.Add(CombatSprintMontage->Montage);
		}
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
	// PA_LOG(LogTemp, Log, TEXT("RightDirection = %f %f %f"), RightDirection.X, RightDirection.Y, RightDirection.Z);

	FVector ForwardDirectionScale = ForwardDirection * MovementVector.X;
	FVector RightDirectionScale = RightDirection * MovementVector.Y;

	FVector DirVec = ForwardDirectionScale + RightDirectionScale;

	SetActorLocationAndRotation(GetActorLocation(), DirVec.ToOrientationRotator());
}

void APAPlayerRMCharacter::ThirdLook(const FInputActionValue& InPutValue)
{
	FVector2D LookAxisVector = InPutValue.Get<FVector2D>();
	// PA_LOG(LogTemp, Log, TEXT("LookAxisVector X= %f /  Y = %f"), LookAxisVector.X, LookAxisVector.Y);

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

void APAPlayerRMCharacter::LightAttack(const FInputActionValue& InPutValue)
{
	uint8 bIsLightAttackInput = InPutValue.Get<bool>();
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	bUseControllerRotationYaw = false;
	FName RowKey = TEXT("Katana_4");
	AttackMontage = PlayerAttackDataTable->FindRow<FPlayerAttackMontage>(RowKey, "5", true);
	FString AttackSection = FString::Printf(TEXT("LightAttack%02d_Start"), CurrentAttackIndx);

	// FString AttackSection = "LightAttack%02d_Start" + FString::FromInt(CurrentAttackIndx);

	// 공격이 재생중인지?
	if (GetMesh()->GetAnimInstance()->Montage_IsActive(AttackMontage->Montage))
	{
		PA_LOG(LogTemp, Log, TEXT("InCombozone Prev Check"));

		bIsNextAttackInput = true;
		// 콤보 존이면 다음 재생 준비
		// if (IsNextAttackPossible())
		//{
		//	PA_LOG(LogTemp, Log, TEXT("Combozone Success Input!!!"));

		//}
		// else
		//{
		//	CurrentAttackIndx = 0;
		//}
	}
	else	// 첫번쨰공격 실행
	{
		CurrentAttackIndx = 0;
		FString InitAttackSection = FString::Printf(TEXT("LightAttack%02d_Start"), CurrentAttackIndx);
		PlayAnimMontage(AttackMontage->Montage, 1.f, *InitAttackSection);
		bIsNextAttackInput = false;
	}
}

void APAPlayerRMCharacter::HeavyAttack(const FInputActionValue& InPutValue)
{
	bUseControllerRotationYaw = false;
}

void APAPlayerRMCharacter::SprintStart(const FInputActionValue& InPutValue)
{
	bIsSprint = InPutValue.Get<bool>();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	for (auto MovementMontage : MovementMontageArray)
	{
		if (!MovementMontage)
		{
			return;
		}
		if (!AnimInstance->Montage_IsPlaying(WalkMontage->Montage))
		{
			return;
		}
	}

	FString MontageSection = AnimInstance->Montage_GetCurrentSection(GetCurrentMontage()).ToString();
	if (MontageSection.Find("End_") != INDEX_NONE)
	{
		return;
	}

	if (GetCurrentMontage()->GetName().Find("Walk"))
	{
		PlayAnimMontage(SprintMontage->Montage, 1.f, FName(CurrentMoveMontageSection));
	}
}

void APAPlayerRMCharacter::SprintEnd(const FInputActionValue& InPutValue)
{
	bIsSprint = InPutValue.Get<bool>();

	for (auto MovementMontage : MovementMontageArray)
	{
		if (!MovementMontage)
		{
			return;
		}
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance->Montage_IsPlaying(WalkMontage->Montage) && !AnimInstance->Montage_IsPlaying(SprintMontage->Montage))
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
		PlayAnimMontage(WalkMontage->Montage, 1.f, FName(CurrentMoveMontageSection));
	}
}

void APAPlayerRMCharacter::MoveStart()
{
	PA_LOG(LogTemp, Log, TEXT("MoveStart() In"));
	CurrentMoveMontageSection = TEXT("Start_Start");
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	uint8 CurrentMovementState = uint8(bIsCombatMode) * 2 + uint8(bIsSprint);
	PA_LOG(LogTemp, Log, TEXT("Current Playing Montage State = %d"), CurrentMovementState);

	if (FName(CurrentMoveMontageSection) == AnimInstance->Montage_GetCurrentSection(MovementMontageArray[CurrentMovementState]))
	{
		PA_LOG(LogTemp, Log, TEXT("이미 Start Anim 재생 중....."));
		return;
	}
	PlayAnimMontage(MovementMontageArray[CurrentMovementState], 1.f, FName(*CurrentMoveMontageSection));
}

void APAPlayerRMCharacter::Move()
{
	CurrentMoveMontageSection = TEXT("Loop_Start");
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
	UAnimMontage* NextMontage = nullptr;
	uint8 CurrentMovementState = uint8(bIsCombatMode) * 2 + uint8(bIsSprint);
	PA_LOG(LogTemp, Log, TEXT("Current Playing Montage State = %d"), CurrentMovementState);

	if (FName(CurrentMoveMontageSection) == AnimInstance->Montage_GetCurrentSection(MovementMontageArray[CurrentMovementState]))
	{
		PA_LOG(LogTemp, Log, TEXT("이미 Start Anim 재생 중....."));
		return;
	}
	PlayAnimMontage(MovementMontageArray[CurrentMovementState], 1.f, FName(*CurrentMoveMontageSection));
}

void APAPlayerRMCharacter::MoveEnd()
{
	CurrentMoveMontageSection = "End_Start";
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UAnimMontage* CurrentMonatage = AnimInstance->GetCurrentActiveMontage();
	FString PrevMontageSection = AnimInstance->Montage_GetCurrentSection().ToString();
	if (CurrentMoveMontageSection == PrevMontageSection)
	{
		return;
	}

	AnimInstance->Montage_Stop(0.5f, CurrentMonatage);
	PlayAnimMontage(CurrentMonatage, 1.f, FName(*CurrentMoveMontageSection));
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
	CurrentAttackIndx = 0;
	KatanaSwordSocket->SetVisibility(false);
	KatanaSheathInSocket->SetVisibility(true);
}

void APAPlayerRMCharacter::Log(EPALogTpye InLogLevel, FString InMessage)
{
	Log(InLogLevel, InMessage, EPALogOutput::ALL);
}

void APAPlayerRMCharacter::Log(EPALogTpye InLogLevel, FString InMessage, EPALogOutput InLogOutput)
{
	// only print when screen is selected and the GEngine object is available
	if ((InLogOutput == EPALogOutput::ALL || InLogOutput == EPALogOutput::SCREEN) && GEngine)
	{
		// default color
		FColor LogColor = FColor::Cyan;
		// flip the color based on the type
		switch (InLogLevel)
		{
			case EPALogTpye::TRACE:
				LogColor = FColor::Green;
				break;
			case EPALogTpye::DEBUG:
				LogColor = FColor::Cyan;
				break;
			case EPALogTpye::INFO:
				LogColor = FColor::White;
				break;
			case EPALogTpye::WARNING:
				LogColor = FColor::Yellow;
				break;
			case EPALogTpye::ERROR:
				LogColor = FColor::Red;
				break;
			default:
				break;
		}
		// print the message and leave it on screen ( 4.5f controls the duration )
		GEngine->AddOnScreenDebugMessage(-1, 4.5f, LogColor, InMessage);
	}

	if (InLogOutput == EPALogOutput::ALL || InLogOutput == EPALogOutput::OUTPUT_LOG)
	{
		// flip the message type based on error level
		switch (InLogLevel)
		{
			case EPALogTpye::TRACE:
				UE_LOG(LogTemp, VeryVerbose, TEXT("%s"), *InMessage);
				break;
			case EPALogTpye::DEBUG:
				UE_LOG(LogTemp, Verbose, TEXT("%s"), *InMessage);
				break;
			case EPALogTpye::INFO:
				UE_LOG(LogTemp, Log, TEXT("%s"), *InMessage);
				break;
			case EPALogTpye::WARNING:
				UE_LOG(LogTemp, Warning, TEXT("%s"), *InMessage);
				break;
			case EPALogTpye::ERROR:
				UE_LOG(LogTemp, Error, TEXT("%s"), *InMessage);
				break;
			default:
				UE_LOG(LogTemp, Log, TEXT("%s"), *InMessage);
				break;
		}
	}
}

void APAPlayerRMCharacter::OnAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}
