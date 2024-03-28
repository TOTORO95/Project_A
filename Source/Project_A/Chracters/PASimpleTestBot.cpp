// Fill out your copyright notice in the Description page of Project Settings.

#include "Chracters/PASimpleTestBot.h"

APASimpleTestBot::APASimpleTestBot()
{
	PlayRate = 1.0f;
	SetupWeapon();
}

void APASimpleTestBot::BeginPlay()
{
	Super::BeginPlay();
	ReturnLocationVector = GetActorLocation();
	ReturnRotator = GetActorRotation();

	BotAnimInstance = GetMesh()->GetAnimInstance();
}

void APASimpleTestBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!BotAnimInstance || !AnimMontage)
	{
		return;
	}
	
	if(BotAnimInstance->Montage_IsPlaying(AnimMontage))
	{
		return;		
	}

	PlayAnimMontage(AnimMontage, PlayRate);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APASimpleTestBot::ReturnLocation);
	BotAnimInstance->Montage_SetEndDelegate(EndDelegate, AnimMontage);

}

void APASimpleTestBot::ReturnLocation(UAnimMontage* InMontage, bool IsProperlyEnded)
{
	TeleportTo(ReturnLocationVector, ReturnRotator);
}

void APASimpleTestBot::MountingWeapon()
{
	KatanaSwordSocket->SetVisibility(true);
	KatanaSheathInSocket->SetVisibility(false);
}

void APASimpleTestBot::ReleaseWeapon()
{
	KatanaSwordSocket->SetVisibility(false);
	KatanaSheathInSocket->SetVisibility(true);
}


void APASimpleTestBot::SetupWeapon()
{
	KatanaSwordSocket = CreateDefaultSubobject<UStaticMeshComponent>("KatanaSword");
	if (GetMesh()->DoesSocketExist("Katana_weapon_01"))
	{
		KatanaSwordSocket->SetupAttachment(GetMesh(), "Katana_weapon_01");
	}

	KatanaSheathSocket = CreateDefaultSubobject<UStaticMeshComponent>("KatanaSheathSocket");
	if (GetMesh()->DoesSocketExist("Katana_sheath_01"))
	{
		KatanaSheathSocket->SetupAttachment(GetMesh(), "Katana_sheath_01");
	}

	KatanaSheathInSocket = CreateDefaultSubobject<UStaticMeshComponent>("KatanaSheathInSocket");
	if (GetMesh()->DoesSocketExist("Katana_sheath_02"))
	{
		KatanaSheathInSocket->SetupAttachment(GetMesh(), "Katana_sheath_02");
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> KatanaSwordMesh = (TEXT("/Game/PA/Item/Weapon/Katana_sword.Katana_sword"));
	if (KatanaSwordMesh.Succeeded())
	{
		KatanaSheathInSocket->SetStaticMesh(KatanaSwordMesh.Object);
		KatanaSwordSocket->SetStaticMesh(KatanaSwordMesh.Object);
		KatanaSwordSocket->SetVisibility(false);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> KatanaSheathMesh = (TEXT("/Game/PA/Item/Weapon/Katana_sheath.Katana_sheath"));
	if (KatanaSheathMesh.Succeeded())
	{
		KatanaSheathSocket->SetStaticMesh(KatanaSheathMesh.Object);
	}
}