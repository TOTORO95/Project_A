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
	auto Checkfunc = [](TObjectPtr <UButton>Btn) 
	{
		if (!ensure(Btn != nullptr))
		{
			return false;
		}
		return true;
	};

	bool bIsEnable = false;
	bIsEnable = Checkfunc(BtnHost);
	bIsEnable = Checkfunc(BtnJoin);
	bIsEnable = Checkfunc(BtnIPJoin);
	bIsEnable = Checkfunc(BtnCloseJoin);
	bIsEnable = Checkfunc(BtnQuitsGame);
	if (!bIsEnable)
	{
		return bIsEnable;
	}

	BtnHost->OnClicked.AddDynamic(this, &UPASteamWidget::HostServer);
	BtnJoin->OnClicked.AddDynamic(this, &UPASteamWidget::OpenJoinMenu);
	BtnIPJoin->OnClicked.AddDynamic(this, &UPASteamWidget::JoinFromIpAddress);
	BtnCloseJoin->OnClicked.AddDynamic(this, &UPASteamWidget::CloseJoinMenu);
	BtnQuitsGame->OnClicked.AddDynamic(this, &UPASteamWidget::QuitsGame);
	return bIsEnable;
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

void UPASteamWidget::QuitsGame()
{
	bool bIsEnable = false;

	auto Checkfunc = [](auto ptr)
	{
		if (!ensure(ptr != nullptr))
		{
			return false;
		}
		return true;
	};

	TObjectPtr<UWorld> World = GetWorld();
	bIsEnable = Checkfunc(World);

	TObjectPtr<APlayerController> PlayerController = World->GetFirstPlayerController();
	bIsEnable = Checkfunc(PlayerController);

	if (!bIsEnable)
	{
		return;
	}
	PlayerController->ConsoleCommand(("quit"));
}
