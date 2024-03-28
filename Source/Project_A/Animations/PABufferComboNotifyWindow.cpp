// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PABufferComboNotifyWindow.h"


#include "Chracters/PAPlayerRMCharacter.h"
#include "Project_A.h"

void UPABufferComboNotifyWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
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

void UPABufferComboNotifyWindow::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
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
		PA_LOG(LogTemp, Log, TEXT("Add Success: Buffer NextAttack!!!"));	
	}
}

void UPABufferComboNotifyWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!MeshComp || !Player)
	{
		return;
	}

	if (bIsPlayerNextAttack)
	{
		PA_LOG(LogTemp, Log, TEXT("Start Buffer NextAttack!!!"));
		Player->NextComboAttack();
	}
	else
	{
		Player->SetNextAttackPossible(false);
	}
}
