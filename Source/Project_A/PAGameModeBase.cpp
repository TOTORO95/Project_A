// Fill out your copyright notice in the Description page of Project Settings.

#include "PAGameModeBase.h"

#include "Chracters/PAPlayerController.h"

APAGameModeBase::APAGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Project_A.PACharacterPlayer"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APAPlayerController> PlayerControllerClassRef(
		TEXT("/Script/Project_A.PAPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
