// Fill out your copyright notice in the Description page of Project Settings.

#include "Animations/PACombaAttackAnimNotifyState.h"

#include "Chracters/PAPlayerRMCharacter.h"
#include "Project_A.h"

void UPACombaAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	// PA_LOG(LogTemp, Log, TEXT("Begin"));
	bIsPlayerNextAttack = false;
	if (!MeshComp)
	{
		return;
	}
	APAPlayerRMCharacter* Player = Cast<APAPlayerRMCharacter>(MeshComp->GetOwner());
	if (!Player)
	{
		return;
	}
	Player->SetNextAttackPossible(true, CurrentComboIndex);
}

void UPACombaAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (!MeshComp)
	{
		return;
	}

	APAPlayerRMCharacter* Player = Cast<APAPlayerRMCharacter>(MeshComp->GetOwner());
	if (!Player)
	{
		return;
	}

	if (!bIsPlayerNextAttack)
	{
		bIsPlayerNextAttack = Player->GetNextAttackInput();
		Player->SetNextAttackPossible(false, CurrentComboIndex);
	}
}

void UPACombaAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!MeshComp)
	{
		return;
	}

	APAPlayerRMCharacter* Player = Cast<APAPlayerRMCharacter>(MeshComp->GetOwner());
	if (!Player)
	{
		return;
	}

	if (bIsPlayerNextAttack)
	{
		PA_LOG(LogTemp, Log, TEXT("StartNextAttack!!!"));
		Player->InitNextAttackInput();
		Player->StartNextAttack();
		bIsPlayerNextAttack = false;
	}
	else
	{
		Player->InitNextAttackInput();
		Player->SetNextAttackPossible(false, 0);
	}
}
