// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Interfaces/PAGameInterface.h"

#include "PAMenuWidget.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_A_API UPAMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetMainMenuInterface(IPAGameInterface* InMenuInterface);
	void Setup();
	void TearDown();

protected:
	IPAGameInterface* MenuInterface;
};
