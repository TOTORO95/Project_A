// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "CoreMinimal.h"

#include "PARootMotionAnimInstance.generated.h"

/**
 *
 */

UCLASS()
class PROJECT_A_API UPARootMotionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPARootMotionAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	uint8 bIsInAir : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	uint8 bIsAnimationBlended : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector CharacterVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float CharacterSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	uint8 bIsCrouching : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	uint8 bIsMoving : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	uint8 bIsSprint : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	uint8 bIsCombatMode : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float MoveForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	uint8 bIsEquipWeapon : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	uint8 bIsDodge : 1;

private:
	APawn* Owner;
};
