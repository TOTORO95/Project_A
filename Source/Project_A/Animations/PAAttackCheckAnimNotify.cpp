// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PAAttackCheckAnimNotify.h"
#include "Chracters/PACharacterBase.h"

void UPAAttackCheckAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}
	APACharacterBase* Character = Cast<APACharacterBase>(MeshComp->GetOwner());
	if (Character)
	{
		Character->AttackHitCheck();
	}
}
