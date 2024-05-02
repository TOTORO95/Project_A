// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PANextPlayAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_A_API UPANextPlayAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FName NextSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float PlayRate;

};
