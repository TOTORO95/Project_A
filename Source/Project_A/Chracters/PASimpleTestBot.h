// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Chracters/PACharacterBase.h"
#include "CoreMinimal.h"

#include "PASimpleTestBot.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_A_API APASimpleTestBot : public APACharacterBase
{
	GENERATED_BODY()
public:
	APASimpleTestBot();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TestBotSetting)
	UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TestBotSetting)
	float PlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TestBotSetting)
	uint8 bIsReturnLocation : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TestBotSetting)
	FVector ReturnLocationVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TestBotSetting)
	FRotator ReturnRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TestBotSetting)
	UAnimInstance* BotAnimInstance;

	void ReturnLocation(class UAnimMontage* InMontage, bool IsProperlyEnded);

	virtual void MountingWeapon() override;
	virtual void ReleaseWeapon() override;

private:
	void SetupWeapon();

	UPROPERTY(EditAnywhere, Category = TestBotSetting)
	UStaticMeshComponent* KatanaSwordSocket;
	UPROPERTY(EditAnywhere, Category = TestBotSetting)
	UStaticMeshComponent* KatanaSheathSocket;
	UPROPERTY(EditAnywhere, Category = TestBotSetting)
	UStaticMeshComponent* KatanaSheathInSocket;
};
