// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstances/PAGameInstance.h"

#include "Interfaces/PAGameInterface.h"
#include "UI/PAInGameMenuWidget.h"
#include "UI/PASteamWidget.h"

UPAGameInstance::UPAGameInstance(const FObjectInitializer& ObjectInitalizer)
{
	InitUI();
}

void UPAGameInstance::Init()
{
	Super::Init();
}

void UPAGameInstance::LoadMenu()
{
	MainMenuHUD = CreateWidget<UPASteamWidget>(this, MainMenuHUDClass);
	if (!ensure(MainMenuHUD))
	{
		return;
	}
	MainMenuHUD->Setup();
	MainMenuHUD->SetMainMenuInterface(this);
}

void UPAGameInstance::LoadInGameMenu()
{
	InGameMenuHUD = CreateWidget<UPAInGameMenuWidget>(this, IngameMenuHUDClass);
	if (!ensure(InGameMenuHUD))
	{
		return;
	}
	InGameMenuHUD->Setup();
	InGameMenuHUD->SetMainMenuInterface(this);
}

void UPAGameInstance::Host()
{
	if (!ensure(GetEngine()) || !ensure(GetWorld()))
	{
		return;
	}
	if (!ensure(MainMenuHUD))
	{
		return;
	}

	GetEngine()->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	MainMenuHUD->TearDown();
	GetWorld()->ServerTravel("/Game/Maps/DevelopMap?listen");
}

void UPAGameInstance::Join(const FString& Address)
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr))
	{
		return;
	}
	GetEngine()->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));
	MainMenuHUD->TearDown();

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPAGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr))
	{
		return;
	}

	PlayerController->ClientTravel("/Game/Maps/MainMenu.MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPAGameInstance::InitUI()
{
	UE_LOG(LogTemp, Warning, TEXT("Init UI"));
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/UI/WBP_MainMenu"));
	if (!ensure(MainMenuBPClass.Class != nullptr))
	{
		return;
	}
	MainMenuHUDClass = MainMenuBPClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/UI/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr))
	{
		return;
	}
	IngameMenuHUDClass = InGameMenuBPClass.Class;
}
