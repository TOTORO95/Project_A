// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "InputActionDataAsset.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_A_API UInputActionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UInputActionDataAsset();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TArray<TObjectPtr<class UInputAction>> InputActions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	FString InputName;
};
