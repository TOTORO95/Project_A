// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "Components/IKFootActorComponent.h"
#include "CoreMinimal.h"

#include "PAAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_A_API UPAAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPAAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

	// IK Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FootIKComponent)
	uint8 bIsIkActive : 1;

private:
	void InitIKFootComponent();
	void UpdateIKFootAnimData();

	UPROPERTY()
	class UIKFootActorComponent* IKFootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IKFootData", meta = (AllowPrivateAccess = "true"))
	FST_IKFootData IKAnimData;
};