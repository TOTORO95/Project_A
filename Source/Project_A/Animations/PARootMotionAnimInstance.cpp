// Fill out your copyright notice in the Description page of Project Settings.

#include "Animations/PARootMotionAnimInstance.h"

#include "Chracters/PAPlayerRMCharacter.h"
#include "Engine.h"
#include "Project_A.h"
#include "GameFramework/PawnMovementComponent.h"

UPARootMotionAnimInstance::UPARootMotionAnimInstance()
{
	bIsInAir = false;
	bIsAnimationBlended = true;
	bIsIdle = true;
	CharacterSpeed = 0.0f;
	bIsCombatMode = false;
	bIsSprint = false;
	MovingThreshould = 3.0f;
}

void UPARootMotionAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// cache the pawn
	Owner = TryGetPawnOwner();
}

void UPARootMotionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// double check our pointers make sure nothing is empty
	if (!Owner)
	{
		return;
	}

	if (Owner->IsA(APAPlayerRMCharacter::StaticClass()))
	{
		APAPlayerRMCharacter* PlayerCharacter = Cast<APAPlayerRMCharacter>(Owner);
		if (PlayerCharacter)
		{
			CharacterVelocity = PlayerCharacter->GetCharacterMovement()->Velocity;
			CharacterSpeed = CharacterVelocity.Size2D();
			bIsInAir = PlayerCharacter->GetMovementComponent()->IsFalling();
			bIsCrouching = PlayerCharacter->GetMovementComponent()->IsCrouching();
			bIsAnimationBlended = PlayerCharacter->IsAnimationBlended();
			bIsEquipWeapon = PlayerCharacter->IsEquipWeapon();
			bIsCombatMode = PlayerCharacter->IsCombatMode();
			bIsCombatMode = PlayerCharacter->IsDodge();
			bIsCombatMode = PlayerCharacter->IsSprint();
			bIsMoving = CharacterSpeed > 0 ? true : false;

			//PA_LOG(LogTemp, Log, TEXT("Character Velocity = %f %f %f"), CharacterVelocity.X, CharacterVelocity.Y, CharacterVelocity.Z);
			//PA_LOG(LogTemp, Log, TEXT("Character Speed = %f"), CharacterSpeed);
			//PA_LOG(LogTemp, Log, TEXT("Character IsMoving = %d"), bIsMoving);
			
			//bIsIdle = CharacterSpeed > 0 ? true : false;
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "IsInAir: " + FString(IsInAir ? "true" : "false"));
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "IsAnimationBlended: " + FString(IsAnimationBlended ? "true" : "false"));
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Speed: " + FString::SanitizeFloat(Speed));
		}
	}
}