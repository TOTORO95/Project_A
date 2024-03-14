// Fill out your copyright notice in the Description page of Project Settings.

#include "Animations/PAAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Project_A.h"
UPAAnimInstance::UPAAnimInstance()
{
	GroundSpeed = 0.0f;
	IKFootComponent = nullptr;
	bIsFalling =false;
	bIsIdle = true;
	bIsJumping = false;
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UPAAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
		InitIKFootComponent();
	}
}

void UPAAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}
	if (IKFootComponent)
	{
		UpdateIKFootAnimData();
	}
}

void UPAAnimInstance::InitIKFootComponent()
{
	UActorComponent* ActorComponent = Owner->GetComponentByClass(UIKFootActorComponent::StaticClass());
	if (ActorComponent)
	{
		// UActorComponent* ActorComponent = PawnOwner->GetComponentByClass(UIKFootActorComponent::StaticClass());
		IKFootComponent = Cast<UIKFootActorComponent>(ActorComponent);
		if (IKFootComponent == nullptr)
		{
			PA_LOG(LogTemp, Warning, TEXT("IK_FootComponent is nullptr"));
		}
	}
}

void UPAAnimInstance::UpdateIKFootAnimData()
{
	IKAnimData = IKFootComponent->GetIKAnimValue();
}
