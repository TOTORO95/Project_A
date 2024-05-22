// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstances/PAGameInstance.h"
#include "Interfaces/PAGameInterface.h"
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
	MainMenuHud = CreateWidget<UPASteamWidget>(this, MainMenuHudClass);
	if (!ensure(MainMenuHud))
	{
		return;
	}
	MainMenuHud->Setup();
	MainMenuHud->SetMainMenuInterface(this);
}

void UPAGameInstance::Host()
{
	if (!ensure(GetEngine()) || !ensure(GetWorld()))
	{
		return;
	}
	if (!ensure(MainMenuHud))
	{
		return;
	}

	GetEngine()->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));
	
	MainMenuHud->TearDown();
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
	MainMenuHud->TearDown();

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPAGameInstance::InitUI()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/UI/WBP_MainMenu"));
	if (!ensure(MainMenuBPClass.Class != nullptr))
	{
		return;
	}
	MainMenuHudClass = MainMenuBPClass.Class;
	UE_LOG(LogTemp, Warning, TEXT("Init UI"));
}
