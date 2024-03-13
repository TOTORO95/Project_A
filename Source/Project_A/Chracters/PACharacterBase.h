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
	// Sets default values for this character's properties
	APACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void InitCharacterComponent();
	virtual void InitControllerRotation();
	virtual void InitCollisionCompoent();
	virtual void InitMovementComponent();

	// Attack Section
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

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;
};
