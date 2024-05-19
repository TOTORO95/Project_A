// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PASteamWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_A_API UPASteamWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize();


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnHost;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnJoin;

	UFUNCTION()
	void HostServer();
};
