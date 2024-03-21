// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PAItemPrimaryDataAsset.h"
#include "PAWeaponPrimaryDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_A_API UPAWeaponPrimaryDataAsset : public UPAItemPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPAWeaponPrimaryDataAsset();
	
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("PAItemData", GetFName());
	}


public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	//UPROPERTY(EditAnywhere, Category = Stat)
	//FPACharacterStat ModifierStat;
};
