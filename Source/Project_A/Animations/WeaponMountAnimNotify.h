// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "WeaponMountAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_A_API UWeaponMountAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = AnimNotify)
	uint8 WeaponMounting : 1;
};

