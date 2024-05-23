// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/PAGameInterface.h"
#include "PAGameInstance.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_A_API UPAGameInstance : public UGameInstance, public IPAGameInterface
{
	GENERATED_BODY()

	UPAGameInstance(const FObjectInitializer& ObjectInitalizer);
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();
	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();
	virtual void Host() override;
	void Join(const FString& Address);
	//virtual void HostServer() override;

protected:
	void InitUI();

private:
	TSubclassOf<class UUserWidget> MainMenuHUDClass;
	TObjectPtr<class UPASteamWidget> MainMenuHUD;

	TSubclassOf<class UUserWidget> IngameMenuHUDClass;
	TObjectPtr<class UPAInGameMenuWidget> InGameMenuHUD;
};
