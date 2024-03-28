// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Chracters/MontageDataAsset.h"
#include "Chracters/PACharacterBase.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "InputActionValue.h"

#include "PAPlayerRMCharacter.generated.h"

/**
 *
 */
#pragma region CustomData

USTRUCT(BlueprintType)
struct FMeleeCollisionProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Enabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Disabled;

	// default constructor
	FMeleeCollisionProfile() : Enabled(TEXT("Weapon")), Disabled(TEXT("NoCollision")){};
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	MELEE_FIST UMETA(DisplayName = "Melee - Fist"),
	MELEE_KICK UMETA(DisplayName = "Melee - Kick")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	FIST UMETA(DisplayName = "Weapone - FIST"),
	GUN UMETA(DisplayName = "Weapone - GUN"),
	SWORD UMETA(DisplayName = "Weapone - SWORD")
};

UENUM(BlueprintType)
enum class EAttackStrength : uint8
{
	LIGHT UMETA(DisplayName = "Light"),
	MEDIUM UMETA(DisplayName = "Medium ( Default )"),
	STRONG UMETA(DisplayName = "Strong")
};

UENUM(BlueprintType)
enum class ELineTraceType : uint8
{
	CAMERA_SINGLE UMETA(DisplayName = "Camera - Single Line"),
	PLAYER_SINGLE UMETA(DisplayName = "Player - Single Line"),
	CAMERA_SCATTER UMETA(DisplayName = "Camera - Scattering"),
	PLAYER_SCATTER UMETA(DisplayName = "Player - Scattering")
};

UENUM(BlueprintType)
enum class EPALogTpye : uint8
{
	TRACE UMETA(DisplayName = "Trace"),
	DEBUG UMETA(DisplayName = "Debug"),
	INFO UMETA(DisplayName = "Info"),
	WARNING UMETA(DisplayName = "Warning"),
	ERROR UMETA(DisplayName = "Error")
};

UENUM(BlueprintType)
enum class EPALogOutput : uint8
{
	ALL UMETA(DisplayName = "All levels"),
	OUTPUT_LOG UMETA(DisplayName = "Output log"),
	SCREEN UMETA(DisplayName = "Screen")
};

UENUM(BlueprintType)
enum class ECharacterControlType : uint8
{
	Third,
	Quater,
	// First,TODO : 아직미구현으로 뒤로 미룸
};

#pragma endregion

UCLASS(config = Game)
class PROJECT_A_API APAPlayerRMCharacter : public APACharacterBase
{
	GENERATED_BODY()
public:
	APAPlayerRMCharacter();

	// Called when the game starts or when the player is spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region InputSection
protected:
	void SetCharacterControl(ECharacterControlType InCharacterControlType);
	void SetCharacterCtrlData(const class UPACharacterCtrlDataAsset* InCharacterCtrlDataAsset);
	void ChangeCharacterControl();
	void SetInputAction();
	void SetMontageTable();

	void ChangeCombatMode(const FInputActionValue& InPutValue);
	void ThirdMove(const FInputActionValue& InPutValue);
	void ThirdLook(const FInputActionValue& InPutValue);
	void QuaterMove(const FInputActionValue& InPutValue);
	void InputAttack(const FInputActionValue& InPutValue);
	void BeginComboAttack();

	void SprintStart(const FInputActionValue& InPutValue);
	void SprintEnd(const FInputActionValue& InPutValue);
	void MoveStart();

public:
	void Move();
	void MoveEnd();

	virtual void MountingWeapon() override;
	virtual void ReleaseWeapon() override;

	void EquipWeaponStart();
	void EquipWeapon();
	void EquipWeaponEnd();
	void CrouchStart();
	void CrouchEnd();
	void SetupKatana();	   // TestCode
						   // void PunchAttack();
						   // void KickAttack();
						   // void AttackInput(EAttackType AttackType);
						   // void WeaponInput(EWeaponType WeaponType);
						   // void AttackStart();
						   // void AttackEnd();

	// void LightAttackStart();
	// void LightAttackEnd();
	// void StrongAttackStart();
	// void StrongAttackEnd();
	void NextComboAttack();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> ChangeControlAciton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> ChangeCombatModeAciton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> ThirdMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> ThirdLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> LightAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> HeavyAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	ECharacterControlType CurrentCharacterControlType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TMap<ECharacterControlType, class UPACharacterCtrlDataAsset*> CharacterControlMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputActionDataAsset> InputActionDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TMap<FString, TObjectPtr<class UInputAction>> InputActionMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	class UDataTable* MovementDataTable;

	UPROPERTY(EditAnywhere, Category = Animation)
	FName AttackRowKey = TEXT("Katana_4");


	TArray<UAnimMontage*> MovementMontageArray;
	FMovementMontage* WalkMontage;
	FMovementMontage* SprintMontage;
	FMovementMontage* CombatWalkMontage;
	FMovementMontage* CombatSprintMontage;

	FString CurrentMoveMontageSection;

#pragma endregion

#pragma region AnimationSection
	/** boolean that tells us if we need to branch our animation blue print paths **/
public:
	UFUNCTION(BlueprintCallable, Category = Animation)
	bool IsAnimationBlended();

	UFUNCTION(BlueprintCallable, Category = Animation)
	bool IsEquipWeapon();

	UFUNCTION(BlueprintCallable, Category = Animation)
	bool IsCombatMode();

	UFUNCTION(BlueprintCallable, Category = Animation)
	bool IsSprint();

	UFUNCTION(BlueprintCallable, Category = Animation)
	bool IsDodge();

	void SetNextAttackPossible(bool InNextAttackPossible);
	bool IsNextAttackPossible();
	bool IsNextAttackInput();
	void SetNextAttackInput(bool InNextAttackInput);
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;


	UStaticMeshComponent* KatanaSwordSocket;
	UStaticMeshComponent* KatanaSheathSocket;
	UStaticMeshComponent* KatanaSheathInSocket;


private:

	UAudioComponent* PunchAudioComponent;
	FPlayerAttackMontage* AttackMontage;
	FMeleeCollisionProfile MeleeCollisionProfile;
	EAttackType CurrentAttack;
	EWeaponType CurrentWeaponType;
	EAttackStrength CurrentAttackStrength;
	
	uint8 bIsAnimationBlended : 1;
	uint8 bIsKeyboardEnabled : 1;
	uint8 bIsLightAttack : 1;
	uint8 bIsHeavyAttack : 1;
	uint8 bIsEquipWeapon : 1;
	uint8 bIsCombatMode : 1;
	uint8 bIsSprint : 1;
	uint8 bIsDodge : 1;
	uint8 bIsNextAttackPossible : 1;
	uint8 bIsNextAttackInput : 1;

	int32 CurrentAttackIndex;

	FTimerHandle CombatToIdleTimerHandle;

	int32 CountdownToIdle;
	int32 CurrentCombo;

#pragma endregion

public:
	UFUNCTION()
	void OnAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** controls if the keyboard responds to user input **/
	UFUNCTION(BlueprintCallable, Category = Animation)
	void SetIsKeyboardEnabled(bool Enabled);

	/** returns the current attack the player is performing **/
	UFUNCTION(BlueprintCallable, Category = Animation)
	EAttackType GetCurrentAttack();

	UFUNCTION(BlueprintCallable, Category = Animation)
	EWeaponType GetCurrentWeaponType();

	UFUNCTION()
	void ResetCombo();

	// UFUNCTION()
	// void TriggerCountdownToIdle();
	// UFUNCTION()
	// void ArmPlayer();
	// void FireLineTrace();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Trace)
	ELineTraceType LineTraceType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Trace)
	float LineTraceDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Trace)
	float LineTraceSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Trace)
	int32 MaxCountdownToIdle;

protected:
	// void MoveForward(float Value);
	// void MoveRight(float Value);
	// void TurnAtRate(float Rate);
	// void LookUpAtRate(float Rate);
	// void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	// void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const
	{
		return CameraBoom;
	}
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const
	{
		return FollowCamera;
	}

	UAudioComponent* PunchThrowAudioComponent;
	FTimerHandle ComboResetTimerHandle;

private:
	void SetupCamera();
	void SetupCollisionBox();
	void SetupSound();
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** melee attack data table **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	class UDataTable* PlayerAttackDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	class USoundCue* PunchSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	class USoundCue* PunchThrowSoundCue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* LeftMeleeCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* RightMeleeCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	float AnimationVariable;
};
