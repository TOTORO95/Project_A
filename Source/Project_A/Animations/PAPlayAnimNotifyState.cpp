// Fill out your copyright notice in the Description page of Project Settings.

#include "Animations/PAPlayAnimNotifyState.h"

#include "Chracters/PAPlayerRMCharacter.h"
#include "Project_A.h"

void UPAPlayAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	PA_LOG(LogTemp, Log, TEXT("Begin"));
	APAPlayerRMCharacter* Player = Cast<APAPlayerRMCharacter>(MeshComp->GetOwner());
	if (!Player)
	{
		return;
	}
	Player->Move();
}

