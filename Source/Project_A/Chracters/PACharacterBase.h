// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "PACharacterBase.generated.h"

UCLASS()
class PROJECT_A_API APACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	APACharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Deprecated Code	*/
protected:
	virtual void InitCharacterComponent();
	virtual void InitControllerRotation();
	virtual void InitCollisionCompoent();
	virtual void InitMovementComponent();

public:
	virtual void MountingWeapon();
	virtual void ReleaseWeapon();
	virtual void AttackHitCheck();
	virtual void PlayNextSectionMontage(class UAnimMontage* Montage, float PlayRate, FName NextSection);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimationMontage)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimationMontage, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UPAComboAttackDataAsset> ComboActionDataAsset;

	void ProcessComboCommand();
	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsPropelyEnded);
	void SetComboCheckTimer();
	void ComboCheck();
	TObjectPtr<class UAnimInstance> GetAnimInstance();
	
	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;
	/* Deprecated Code	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK_Foot", meta = (AllowPrivateAccess = "true"))
	class UIKFootActorComponent* IKFootComponent;
};
