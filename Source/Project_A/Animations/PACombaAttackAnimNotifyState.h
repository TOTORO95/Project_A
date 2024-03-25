// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CoreMinimal.h"

#include "PACombaAttackAnimNotifyState.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_A_API UPACombaAttackAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = AnimNotify)
	uint8 CurrentComboIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = AnimNotify)
	uint8 NextAttackEnable : 1;

	uint8 bIsPlayerNextAttack : 1;
};