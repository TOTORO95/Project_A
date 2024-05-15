// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PAGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_A_API APAGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	APAGameModeBase();
	virtual void StartPlay() override;

	//virtual FTransform GetRandomStartTransform() const override;
	//virtual void OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn) override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void DefaultGameTimer();
	void FinishMatch();
	FTimerHandle GameTimeHandle;

protected:
	TArray<TObjectPtr<class APlayerStart>> PlayerStartArray;
};
