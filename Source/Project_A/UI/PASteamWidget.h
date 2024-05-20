// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Interfaces/PAGameInterface.h"

#include "PASteamWidget.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_A_API UPASteamWidget : public UUserWidget
{
	GENERATED_BODY()
	public:
	void SetMainMenuInterface(IPAGameInterface* InMenuInterface);

protected:
	virtual bool Initialize();

private:
	UFUNCTION()
	void HostServer();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnHost;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnJoin;

	IPAGameInterface* MenuInterface;
};
