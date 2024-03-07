// Fill out your copyright notice in the Description page of Project Settings.

#include "PAGameModeBase.h"

#include "Chracters/PAPlayerController.h"
#include "Project_A.h"

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
void APAGameModeBase::StartPlay()
{
	PA_LOG(LogPANetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::StartPlay();
}

void APAGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//GetWorldTimerManager().SetTimer(
	//	GameTimeHandle, this, &APAGameModeBase::DefaultGameTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void APAGameModeBase::DefaultGameTimer()
{
}

void APAGameModeBase::FinishMatch()
{
}
