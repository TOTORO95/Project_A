// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/WeaponMountAnimNotify.h"
#include "Chracters/PAPlayerRMCharacter.h"
#include "Project_A.h"

void UWeaponMountAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	APAPlayerRMCharacter* Player = Cast<APAPlayerRMCharacter>(MeshComp->GetOwner());
	if (!Player)
	{
		return;
	}

	if (WeaponMounting)
	{
		Player->MountingWeapon();
	}
	else
	{
		Player->ReleaseWeapon();	
	}
}
