// Fill out your copyright notice in the Description page of Project Settings.

#include "Animations/PANextPlayAnimNotify.h"

#include "Chracters/PACharacterBase.h"

void UPANextPlayAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}
	APACharacterBase* Character = Cast<APACharacterBase>(MeshComp->GetOwner());

	if (!Character)
	{
		return;
	}
	UAnimMontage* Montage = Cast<UAnimMontage>(Animation);
	if (!Montage)
	{
		return;
	}

	Character->PlayNextSectionMontage(Montage, PlayRate, NextSection);
}
