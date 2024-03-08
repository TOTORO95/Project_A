// Fill out your copyright notice in the Description page of Project Settings.

#include "PAGameModeBase.h"

#include "Chracters/PAPlayerController.h"
#include "Project_A.h"

APAGameModeBase::APAGameModeBase()
{
	///Script/Engine.Blueprint'/Game/PA/Characters/Blueprints/BP_ThirdPersonPhase.BP_ThirdPersonPhase'
	///Game/PA/Characters/Blueprints/BP_ThirdPersonPhase.BP_ThirdPersonPhase

	///Script/Engine.Blueprint'/Game/PA/Characters/Blueprints/BP_ThirdPersonPhase_2.BP_ThirdPersonPhase_2'
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/Ninja_Anims/Demo/Mannequin/Character/Mesh/BP_Ninja_Mann_2.BP_Ninja_Mann_2_C"));

	//static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/PA/Characters/Blueprints/BP_ThirdPersonPhase_2.BP_ThirdPersonPhase_2_C"));
	//static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Project_A.PACharacterPlayer"));
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
