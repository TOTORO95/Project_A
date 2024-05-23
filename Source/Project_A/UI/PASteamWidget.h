// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "UI/PAMenuWidget.h"

#include "PASteamWidget.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_A_API UPASteamWidget : public UPAMenuWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void JoinFromIpAddress();
	UFUNCTION()
	void CloseJoinMenu();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnHost;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnJoin;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Btn_CloseJoin;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnIPJoin;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidget> JoinMenu;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidget> MainMenu;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> MenuSwitcher;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableText> IPAddressField;
};
