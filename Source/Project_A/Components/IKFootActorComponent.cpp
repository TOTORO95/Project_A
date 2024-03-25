// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/IKFootActorComponent.h"

#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Project_A.h"

UIKFootActorComponent::UIKFootActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Character = nullptr;
	IKCapsuleHalfHeight = 0.0f;
	IKFeetInterpSpeed = 22.0f;
	IKHipsInterpSpeed = 17.0f;
	IKTraceDistance = 55.0f;
	IKAdjustOffset = 5.0f;
	IkDeltaTime = 0.0f;
	bIsDebug = true;
	IgnoreIkFootValue = 0.1f;
	IgnoreIkHipValue = 0.1f;
}

void UIKFootActorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr)
	{
		return;
	}
	IKCapsuleHalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

void UIKFootActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Character = nullptr;
}

// Called every frame
void UIKFootActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!Character || !IsActive())
		return;

	IK_Update(DeltaTime);
	IK_Debug();
}

void UIKFootActorComponent::Set_IKSocketName(FString InSocketName_LeftFoot, FString InSocketName_RightFoot)
{
	IKSocketName_LeftFoot = FName(*InSocketName_LeftFoot);
	IKSocketName_RightFoot = FName(*InSocketName_RightFoot);
}

void UIKFootActorComponent::IK_ResetVars()
{
	// All reset the Animvalues
	IKFootData.DistanceRequired_Left = UKismetMathLibrary::FInterpTo(0.0f, IKFootData.DistanceRequired_Left, IkDeltaTime, IKFeetInterpSpeed);
	IKFootData.DistanceRequired_Right = UKismetMathLibrary::FInterpTo(0.0f, IKFootData.DistanceRequired_Right, IkDeltaTime, IKFeetInterpSpeed);

	IKFootData.FootRotationRequired_Left = UKismetMathLibrary::RInterpTo(IKFootData.FootRotationRequired_Left, FRotator::ZeroRotator, IkDeltaTime, IKFeetInterpSpeed);
	IKFootData.FootRotationRequired_Right = UKismetMathLibrary::RInterpTo(IKFootData.FootRotationRequired_Right, FRotator::ZeroRotator, IkDeltaTime, IKFeetInterpSpeed);

	IKFootData.RequiredOffset_Hip = UKismetMathLibrary::FInterpTo(IkDeltaTime, 0.0f, IKFootData.RequiredOffset_Hip, IKHipsInterpSpeed);
	IK_Update_CapsuleHalfHeight(IkDeltaTime, 0.0f, true);
}

void UIKFootActorComponent::IK_Update(float InDeltaTime)
{
	IkDeltaTime = InDeltaTime;

	// Get Line Trace Info of ground from foot
	ST_IKTraceInfo Trace_Left = IK_FootTrace(IKTraceDistance, IKSocketName_LeftFoot);
	ST_IKTraceInfo Trace_Right = IK_FootTrace(IKTraceDistance, IKSocketName_RightFoot);

	FRotator TargetRotator_Left = NormalToRotator(Trace_Left.DistanceRequired);
	FRotator TargetRotator_Right = NormalToRotator(Trace_Right.DistanceRequired);

	// if (IsMoving())
	//{
	//	TargetRotator_Left = FRotator::ZeroRotator;
	//	TargetRotator_Right = FRotator::ZeroRotator;
	// }

	IKFootData.FootRotationRequired_Left = UKismetMathLibrary::RInterpTo(IKFootData.FootRotationRequired_Left, TargetRotator_Left, InDeltaTime, IKFeetInterpSpeed);
	IKFootData.FootRotationRequired_Right = UKismetMathLibrary::RInterpTo(IKFootData.FootRotationRequired_Right, TargetRotator_Right, InDeltaTime, IKFeetInterpSpeed);
	float HipsOffset = UKismetMathLibrary::Min(Trace_Left.Offset, Trace_Right.Offset);
	HipsOffset = FMath::Clamp(HipsOffset, HipsOffset, 0.0f);

	IKFootData.RequiredOffset_Hip = UKismetMathLibrary::FInterpTo(IKFootData.RequiredOffset_Hip, HipsOffset, InDeltaTime, IKHipsInterpSpeed);
	IK_Update_CapsuleHalfHeight(InDeltaTime, HipsOffset, false);

	// Update effector locations of foot
	IKFootData.DistanceRequired_Left = UKismetMathLibrary::FInterpTo(IKFootData.DistanceRequired_Left, Trace_Left.Offset - HipsOffset, InDeltaTime, IKFeetInterpSpeed);
	IKFootData.DistanceRequired_Right = UKismetMathLibrary::FInterpTo(IKFootData.DistanceRequired_Right, -1 * (Trace_Right.Offset - HipsOffset), InDeltaTime, IKFeetInterpSpeed);

	// Character vibration prevention
	CheckIgnoreValue(PrevIKFootData, IKFootData);

	// When foot is close to trace effector location disable IK
	// bool bLeftFootNearly = UKismetMathLibrary::NearlyEqual_FloatFloat(IKAnimValue.EffectorLocation_Left, Trace_Left.Offset - fHipsOffset, 1.0f);
	// bool bRightFootNearly = UKismetMathLibrary::NearlyEqual_FloatFloat(IKAnimValue.EffectorLocation_Right, Trace_Right.Offset - fHipsOffset, 1.0f);
	// if (bLeftFootNearly && bRightFootNearly)
	//{
	//	PA_LOG(LogTemp, Log, TEXT("SetIKActive = false"));
	//	SetIKActive(false);
	//}
	// else
	//{
	//	PA_LOG(LogTemp, Log, TEXT("SetIKActive = true"));
	//	SetIKActive(true);
	//}
}

void UIKFootActorComponent::IK_Debug()
{
	if (bIsDebug)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Foot_L Require Distansce(Z) : " + FString::SanitizeFloat(IKFootData.DistanceRequired_Left), true, false, FLinearColor::Red, 0.0f);
		UKismetSystemLibrary::PrintString(GetWorld(), "Foot_R Require Distansce(Z) : " + FString::SanitizeFloat(IKFootData.DistanceRequired_Right), true, false, FLinearColor::Red, 0.0f);
		UKismetSystemLibrary::PrintString(GetWorld(), "Foot_L Require Rotation : " + IKFootData.FootRotationRequired_Left.ToString(), true, false, FLinearColor::Red, 0.0f);
		UKismetSystemLibrary::PrintString(GetWorld(), "Foot_R Require Rotation : " + IKFootData.FootRotationRequired_Right.ToString(), true, false, FLinearColor::Red, 0.0f);
		UKismetSystemLibrary::PrintString(GetWorld(), "HipOffset(Z) : " + FString::SanitizeFloat(IKFootData.RequiredOffset_Hip), true, false, FLinearColor::Red, 0.0f);
	}

	if (Character)
	{
		if (Character->GetCapsuleComponent()->bHiddenInGame == bIsDebug)
		{
			Character->GetCapsuleComponent()->bHiddenInGame = !bIsDebug;
		}
	}
}

void UIKFootActorComponent::SetIKDebug(bool InbIsDebug)
{
	bIsDebug = InbIsDebug;
}

void UIKFootActorComponent::IK_Update_CapsuleHalfHeight(float InDeltaTime, float InHipShift, bool InbResetDefault)
{
	UCapsuleComponent* pCapsule = Character->GetCapsuleComponent();
	if (pCapsule == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("IK : Capsule is NULL"));
		return;
	}

	// Get Half Height of capsule component
	float CapsuleHalfHeight = 0.0f;
	if (InbResetDefault == true)
	{
		CapsuleHalfHeight = IKCapsuleHalfHeight;
	}
	else
	{
		float fHalfAbsSize = UKismetMathLibrary::Abs(InHipShift) * 0.5f;
		CapsuleHalfHeight = IKCapsuleHalfHeight - fHalfAbsSize;
	}

	// Set capsule component height with FInterpTo
	float ScaledCapsuleHalfHeight = pCapsule->GetScaledCapsuleHalfHeight();
	float InterpValue = UKismetMathLibrary::FInterpTo(ScaledCapsuleHalfHeight, CapsuleHalfHeight, InDeltaTime, IKHipsInterpSpeed);

	pCapsule->SetCapsuleHalfHeight(InterpValue, true);
}

ST_IKTraceInfo UIKFootActorComponent::IK_FootTrace(float InTraceDistance, FName InSocketName)
{
	ST_IKTraceInfo IKTraceInfo;

	// Set Linetraces startpoint and end point
	FVector SocketLocation = Character->GetMesh()->GetSocketLocation(InSocketName);
	FVector Line_Start = FVector(SocketLocation.X, SocketLocation.Y, Character->GetActorLocation().Z);
	FVector Line_End = FVector(SocketLocation.X, SocketLocation.Y, (Character->GetActorLocation().Z - IKCapsuleHalfHeight) - InTraceDistance);

	// Process Line Trace
	FHitResult HitResult;
	TArray<AActor*> IgnoreActor;
	IgnoreActor.Add(GetOwner());

	EDrawDebugTrace::Type eDebug = EDrawDebugTrace::None;
	if (bIsDebug)
	{
		eDebug = EDrawDebugTrace::ForOneFrame;
	}

	bool bResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Line_Start, Line_End, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, IgnoreActor, eDebug, HitResult, true);

	// Set ImpactNormal and Offset from HitResult
	IKTraceInfo.DistanceRequired = HitResult.ImpactNormal;
	if (HitResult.IsValidBlockingHit())
	{
		float ImpactLegth = (HitResult.ImpactPoint - HitResult.TraceEnd).Size();
		IKTraceInfo.Offset = IKAdjustOffset + (ImpactLegth - InTraceDistance);
	}
	else
	{
		IKTraceInfo.Offset = 0.0f;
	}

	return IKTraceInfo;
}

bool UIKFootActorComponent::IsMoving()
{
	float CharacterSpeed = Character->GetVelocity().Size();
	if (CharacterSpeed > 0.0f)
	{
		return true;
	}
	return false;
}

void UIKFootActorComponent::CheckIgnoreValue(FST_IKFootData& InPrevValue, FST_IKFootData& InCurValue)
{
	auto Func = [this](float& PrevVal, float& CurVal, bool bIsFoot) 
	{
		float IgnoreVal = IgnoreIkHipValue;
		if (bIsFoot)
		{
			IgnoreVal = IgnoreIkFootValue;
		}

		float DistanceDiff = FMath::Abs(PrevVal - CurVal);
		if (DistanceDiff > IgnoreVal)
		{
			PrevVal = CurVal;
		}
		else
		{
			CurVal = PrevVal;
		}
	};
	Func(InPrevValue.DistanceRequired_Left, InCurValue.DistanceRequired_Left, true);
	Func(InPrevValue.DistanceRequired_Right, InCurValue.DistanceRequired_Right, true);
	Func(InPrevValue.RequiredOffset_Hip, InCurValue.RequiredOffset_Hip, false);
}

FRotator UIKFootActorComponent::NormalToRotator(FVector InVector)
{
	float Atan2_1 = UKismetMathLibrary::DegAtan2(InVector.Y, InVector.Z);
	float Atan2_2 = UKismetMathLibrary::DegAtan2(InVector.X, InVector.Z);
	Atan2_2 *= -1.0f;
	FRotator pResult = FRotator(Atan2_2, 0.0f, Atan2_1);

	return pResult;
}