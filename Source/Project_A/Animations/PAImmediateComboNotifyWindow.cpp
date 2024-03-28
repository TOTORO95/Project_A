// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PAImmediateComboNotifyWindow.h"
#include "Chracters/PAPlayerRMCharacter.h"
#include "Project_A.h"

void UPAImmediateComboNotifyWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (!MeshComp)
	{
		return;
	}

	Player = Cast<APAPlayerRMCharacter>(MeshComp->GetOwner());
	if (!Player)
	{
		return;
	}
	bIsPlayerNextAttack = false;
	Player->SetNextAttackPossible(true);
}

void UPAImmediateComboNotifyWindow::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (!MeshComp || !Player)
	{
		return;
	}

	if (!bIsPlayerNextAttack)
	{
		bIsPlayerNextAttack = Player->IsNextAttackInput();
	}
	else
	{
		Player->NextComboAttack();
	}
}

void UPAImmediateComboNotifyWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!MeshComp || !Player)
	{
		return;
	}
	if (!bIsPlayerNextAttack)
	{
		Player->SetNextAttackPossible(false);
	}
}
