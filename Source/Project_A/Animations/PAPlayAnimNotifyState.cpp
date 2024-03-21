// Fill out your copyright notice in the Description page of Project Settings.

#include "Animations/PAPlayAnimNotifyState.h"
#include "Project_A.h"
#include "Chracters/PAPlayerRMCharacter.h"

void UPAPlayAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	PA_LOG(LogTemp, Log, TEXT("Begin"));


	// if (!MeshComp && !MeshComp->GetOwner())
	//{
	//	return;
	// }

	 APAPlayerRMCharacter* Player = Cast<APAPlayerRMCharacter>(MeshComp->GetOwner());
	 if (!Player)
	{
		return ;
	 }
	 Player->Walk();
}

void UPAPlayAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);


}

void UPAPlayAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	PA_LOG(LogTemp, Log, TEXT("End"));

	//APAPlayerRMCharacter* Player = Cast<APAPlayerRMCharacter>(MeshComp->GetOwner());
	//if (!Player)
	//{
	//	return;
	//}
}

USkeletalMeshComponent* UPAPlayAnimNotifyState::GetMeshComponent(USkeletalMeshComponent* MeshComp)
{
	if (!MeshComp && !MeshComp->GetOwner())
	{
		return nullptr;
	}
	return MeshComp;
}
