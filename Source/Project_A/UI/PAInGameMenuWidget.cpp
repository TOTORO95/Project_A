// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PAInGameMenuWidget.h"

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
	if (!ensure(BtnCanceInGameMenu != nullptr))
	{
		return false;
	}

	return true;
}