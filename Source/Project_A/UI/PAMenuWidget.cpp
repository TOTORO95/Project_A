// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PAMenuWidget.h"

void UPAMenuWidget::SetMainMenuInterface(IPAGameInterface* InMenuInterface)
{
	MenuInterface = InMenuInterface;
}

void UPAMenuWidget::Setup()
{
	AddToViewport();

	TObjectPtr<APlayerController> PlayerController = GetWorld()->GetFirstPlayerController();
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = true;
}

void UPAMenuWidget::TearDown()
{
	RemoveFromParent();
	// RemoveFromViewport();
	TObjectPtr<APlayerController> PlayerController = GetWorld()->GetFirstPlayerController();
	FInputModeGameOnly InputModeData;
	if (!ensure(PlayerController != nullptr))
	{
		return;
	}
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}
