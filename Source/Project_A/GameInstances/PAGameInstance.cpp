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
}

void UPAGameInstance::LoadMenu()
{
	MainMenuHud = CreateWidget<UPASteamWidget>(this, MainMenuHudClass);
	if (!ensure(MainMenuHud != nullptr))
	{
		return;
	}
	MainMenuHud->Setup();
	MainMenuHud->SetMainMenuInterface(this);
}

void UPAGameInstance::Host()
{
	TObjectPtr<UEngine> Engine = GetEngine();
	if (!ensure(Engine != nullptr))
	{
		return;
	}
	TObjectPtr<UWorld> World = GetWorld();
	if(!ensure(World != nullptr))
	{
		return;
	}

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));
	
	MainMenuHud->TearDown();
	World->ServerTravel("/Game/Maps/DevelopMap?listen");
}

void UPAGameInstance::Join(const FString& Address)
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (ensure(PlayerController != nullptr))
	{
		return;
	}
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
