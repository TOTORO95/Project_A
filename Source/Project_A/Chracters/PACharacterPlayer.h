// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Chracters/PACharacterBase.h"
#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "PACharacterPlayer.generated.h"

/**
 *
 */
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

	// Camera Section
protected:
	void ThirdPersonCameraSetting();
	void FirstPersonCameraSetting();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> LookAction;

	void Move(const FInputActionValue& InPutValue);
	void ChangeCameraView(const FInputActionValue& InPutValue);
};
