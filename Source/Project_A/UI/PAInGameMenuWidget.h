// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "UI/PAMenuWidget.h"

#include "PAInGameMenuWidget.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_A_API UPAInGameMenuWidget : public UPAMenuWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

	UFUNCTION()
	void BackMainMenu();
	UFUNCTION()
	void CancelInGameMenu();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnBackMainMenu;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnCancelInGameMenu;
};
