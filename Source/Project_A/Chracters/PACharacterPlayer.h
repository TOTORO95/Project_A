// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Chracters/PACharacterBase.h"
#include "CoreMinimal.h"
#include "InputActionValue.h"

#include "PACharacterPlayer.generated.h"

/**
 *
 */
UENUM()
enum class ECharacterCtrlType : uint8
{
	Third,
	Quater,
	// First,TODO : 아직미구현으로 뒤로 미룸
};

UCLASS()
class PROJECT_A_API APACharacterPlayer : public APACharacterBase
{
	GENERATED_BODY()
public:
	APACharacterPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Character Control
	void ChangeCharacterCtrl();
	void SetCharacterCtrl(ECharacterCtrlType InCharacterControlType);

	void SetCharacterCtrlData(const class UPACharacterCtrlDataAsset* InCharacterCtrlDataAsset);
	UPROPERTY(VisibleAnywhere);
	TMap<ECharacterCtrlType, class UPACharacterCtrlDataAsset*> CharacterCtrlMap;

	// Camera Section
protected:
	void SetupControlDataAsset();
	void SetupCamera();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> ChangeCtrlAciton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> ThirdMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> ThirdLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> AttackAction;

	void SetInputAction();
	void ThirdMove(const FInputActionValue& InPutValue);
	void ThirdLook(const FInputActionValue& InPutValue);
	void QuaterMove(const FInputActionValue& InPutValue);
	void Attack(const FInputActionValue& InPutValue);

	ECharacterCtrlType CurrentCharacterCtrlype;
};
