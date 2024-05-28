// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PAInGameMenuWidget.h"
#include "Components/Button.h"

bool UPAInGameMenuWidget::Initialize()
{
	if (!ensure(Super::Initialize()))
	{
		return false;
	}

	if (!ensure(BtnBackMainMenu != nullptr))
	{
		return false;
	}
	if (!ensure(BtnCancelInGameMenu != nullptr))
	{
		return false;
	}

	BtnBackMainMenu->OnClicked.AddDynamic(this, &UPAInGameMenuWidget::BackMainMenu);
	BtnCancelInGameMenu->OnClicked.AddDynamic(this, &UPAInGameMenuWidget::CancelInGameMenu);

	return true;
}

void UPAInGameMenuWidget::BackMainMenu()
{
	if (!ensure(MenuInterface != nullptr))
	{
		return;
	}
	TearDown();
	MenuInterface->LoadMainMenu();
}

void UPAInGameMenuWidget::CancelInGameMenu()
{
	if (!ensure(MenuInterface != nullptr))
	{
		return;
	}
	TearDown();
}