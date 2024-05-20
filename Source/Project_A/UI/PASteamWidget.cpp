// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PASteamWidget.h"

#include "Components/Button.h"

void UPASteamWidget::SetMainMenuInterface(IPAGameInterface* InMenuInterface)
{
	MenuInterface = InMenuInterface;
}

bool UPASteamWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)
	{
		return false;
	}

	if (!ensure(BtnHost != nullptr))
	{
		return false;
	}

	BtnHost->OnClicked.AddDynamic(this, &UPASteamWidget::HostServer);

	return true;
}

void UPASteamWidget::HostServer()
{
	if (!ensure(MenuInterface!=nullptr))
	{
		return;
	}
	MenuInterface->Host();
}
