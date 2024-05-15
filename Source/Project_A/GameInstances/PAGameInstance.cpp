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
