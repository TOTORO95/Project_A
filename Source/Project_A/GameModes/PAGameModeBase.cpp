// Fill out your copyright notice in the Description page of Project Settings.

#include "PAGameModeBase.h"

#include "Chracters/PAPlayerController.h"
#include "Project_A.h"

APAGameModeBase::APAGameModeBase()
{
	FString BP_PlayerPath = TEXT("/Script/Engine.Blueprint'/Game/PA/Characters/General/BP_PAGenPlayer.BP_PAGenPlayer_C'");
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(*BP_PlayerPath);
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
	PA_NETLOG(LogPANetwork, Log, TEXT("%s"), TEXT("Begin"));
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
