// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "IKFootActorComponent.generated.h"

// Line Trace info struct
typedef struct ST_IKTraceInfo
{
	float Offset;
	FVector DistanceRequired;
};

USTRUCT(Atomic, BlueprintType)
struct FST_IKFootData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator FootRotationRequired_Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator FootRotationRequired_Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceRequired_Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceRequired_Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RequiredOffset_Hip;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_A_API UIKFootActorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class ACharacter* Character;

private:
	// Owner Characters capsule height
	float IKCapsuleHalfHeight;

	// IK Anim Instance Value struct
	FST_IKFootData IKFootData;
	FST_IKFootData PrevIKFootData;

	UPROPERTY(EditAnywhere, Category = "IKFootComponent")
	float IgnoreIkFootValue = 0.0f;

	UPROPERTY(EditAnywhere, Category = "IKFootComponent")
	float IgnoreIkHipValue = 0.0f;

	// IK Active state
	bool bIsActive = false;
	float IkDeltaTime = 0.0f;

public:
	// Left foot bone name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IKFootComponent_Socket")
	FName IKSocketName_LeftFoot;
	// Right foot bone name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IKFootComponent_Socket")
	FName IKSocketName_RightFoot;

	// Distance between the floor and the foot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IKFootComponent")
	float IKAdjustOffset;

	// Max LineTrace distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IKFootComponent")
	float IKTraceDistance;

	// Hip(pelvis) move speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IKFootComponent")
	float IKHipsInterpSpeed;

	// Leg joint move speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IKFootComponent")
	float IKFeetInterpSpeed;

	// Enable debug mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IKFootComponent")
	bool bIsDebug;

public:
	UIKFootActorComponent();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Set foot bone name from skeletal mesh
	void Set_IKSocketName(FString SocName_LeftFoot, FString SocName_RightFoot);

	// Reset IK State
	void IK_ResetVars();

	// Set IK Active state
	void SetIKActive(bool InbActive);

	// Set IK Debug state
	void SetIKDebug(bool InbDebug);

private:
	void IK_Update(float InDeltaTime);
	void IK_Debug();
	void IK_Update_CapsuleHalfHeight(float InDeltaTime, float InHipShift, bool bResetDefault);
	ST_IKTraceInfo IK_FootTrace(float InTraceDistance, FName Socket);

	// Get ground normal
	FRotator NormalToRotator(FVector pVector);
	bool IsMoving();
	void CheckIgnoreValue(FST_IKFootData& InPrevValue, FST_IKFootData& InCurValue);

public:
	FORCEINLINE FST_IKFootData GetIKAnimValue()
	{
		return IKFootData;
	}
	FORCEINLINE bool GetIKDebugState()
	{
		return bIsDebug;
	}
};
