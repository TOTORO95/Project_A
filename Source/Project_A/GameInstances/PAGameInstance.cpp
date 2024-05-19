// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstances/PAGameInstance.h"

UPAGameInstance::UPAGameInstance(const FObjectInitializer& ObjectInitalizer)
{
	InitUI();
}

void UPAGameInstance::Init()
{
}

void UPAGameInstance::LoadMenu()
{
	TObjectPtr<UUserWidget> Menu = CreateWidget<UUserWidget>(this, MainMenuHud);
	if (Menu == nullptr)
	{
		return;
	}
	Menu->AddToViewport();
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (ensure(PlayerController != nullptr))
	{
		return;
	}
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(Menu->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = true;
}

void UPAGameInstance::Host()
{
	TObjectPtr<UEngine> Engine = GetEngine();
	if (Engine == nullptr)
	{
		return;
	}
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	TObjectPtr<UWorld> World = GetWorld();
	if(ensure(World != nullptr))
	{
		return;
	}
	World->ServerTravel("/Game/Maps/DevelopMap?listen");
	//"/Script/Engine.World'/Game/Maps/DevelopMap.DevelopMap'"
}

void UPAGameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();
	if (ensure(Engine != nullptr))
	{
		return;
	}
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
	if (MainMenuBPClass.Class == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Found WBP_MainMenu"));
	}
	else
	{
		MainMenuHud = MainMenuBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Init UI"));
	}
}
