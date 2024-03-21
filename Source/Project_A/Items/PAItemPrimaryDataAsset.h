// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "PAItemPrimaryDataAsset.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon = 0,
	Potion,
	Scroll
};

/**
 *
 */

UCLASS()
class PROJECT_A_API UPAItemPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPAItemPrimaryDataAsset();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("PAItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemType)
	EItemType ItemType;
};
