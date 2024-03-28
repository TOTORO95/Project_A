// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/WeaponMountAnimNotify.h"
#include "Chracters/PACharacterBase.h"

#include "Project_A.h"

void UWeaponMountAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	APACharacterBase* Character = Cast<APACharacterBase>(MeshComp->GetOwner());
	if (!Character)
	{
		return;
	}

	if (WeaponMounting)
	{
		Character->MountingWeapon();
	}
	else
	{
		Character->ReleaseWeapon();	
	}
}
