// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "PAGameInstance.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_A_API UPAGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPAGameInstance(const FObjectInitializer& ObjectInitalizer);
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable) 
	void LoadMenu();

	void Host();

	void Join(const FString& Address);
protected:
	void InitUI();

	TSubclassOf<class UUserWidget> MainMenuHud;
};
