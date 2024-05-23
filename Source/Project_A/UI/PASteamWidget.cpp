// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PASteamWidget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"

bool UPASteamWidget::Initialize()
{
	if (!ensure(Super::Initialize()))
	{
		return false;
	}
	if (!ensure(BtnHost != nullptr))
	{
		return false;
	}
	if (!ensure(BtnJoin != nullptr))
	{
		return false;
	}

	BtnHost->OnClicked.AddDynamic(this, &UPASteamWidget::HostServer);
	BtnJoin->OnClicked.AddDynamic(this, &UPASteamWidget::OpenJoinMenu);
	BtnIPJoin->OnClicked.AddDynamic(this, &UPASteamWidget::JoinFromIpAddress);
	Btn_CloseJoin->OnClicked.AddDynamic(this, &UPASteamWidget::CloseJoinMenu);

	return true;
}

void UPASteamWidget::HostServer()
{
	if (!ensure(MenuInterface != nullptr))
	{
		return;
	}
	MenuInterface->Host();
}

void UPASteamWidget::OpenJoinMenu()
{
	if (!ensure(JoinMenu))
	{
		return;
	}
	MenuSwitcher->SetActiveWidget(JoinMenu);
}
void UPASteamWidget::JoinFromIpAddress()
{
	if (!ensure(IPAddressField) || !ensure(MenuInterface))
	{
		return;
	}
	const FString& IpAddress = IPAddressField->GetText().ToString();
	MenuInterface->Join(IpAddress);
}
void UPASteamWidget::CloseJoinMenu()
{
	if (!ensure(MainMenu))
	{
		return;
	}
	MenuSwitcher->SetActiveWidget(MainMenu);
}
