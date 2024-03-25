// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PASyncRotMontageAnimNotifyState.h"
#include "Chracters/PAPlayerRMCharacter.h"

void UPASyncRotMontageAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

}

void UPASyncRotMontageAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	USkeletalMeshComponent* Mesh = GetMeshComponent(MeshComp);
	 APAPlayerRMCharacter* Player = Cast<APAPlayerRMCharacter>(MeshComp->GetOwner());
	 if (!Player)
	{
		return;
	 }
}

void UPASyncRotMontageAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
