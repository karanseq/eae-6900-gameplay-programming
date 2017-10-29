// Fill out your copyright notice in the Description page of Project Settings.

#include "Destructible.h"

// engine includes
#include "Components/BoxComponent.h"

// game includes
#include "EAE_6900GameInstance.h"

ADestructible::ADestructible(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->SetCollisionProfileName(FName(TEXT("OverlapAllDynamic")));
	BoxComponent->CanCharacterStepUpOn = ECB_No;
	BoxComponent->bDynamicObstacle = true;
	RootComponent = BoxComponent;
}

//~==============================================================================
// Behavior

float ADestructible::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	// reduce health
	++HitsTaken;

	// call blueprint event
	UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	ReceiveAnyDamage(DamageAmount, DamageTypeCDO, EventInstigator, DamageCauser);

	UpdateAppearance();
	SetActorHiddenInGame(HitsTaken >= HitsToDestroy);

	return DamageAmount;
}

void ADestructible::BeginPlay()
{
	ensure(GUID != NAME_None);

	UEAE_6900GameInstance* GameInstance = UEAE_6900GameInstance::GetInstance();
	GameInstance->RegisterSaveableObject(this);

	Super::BeginPlay();

	const FDestructibleSaveData* SavedData = nullptr;
	GameInstance->GetCurrentlyLoadedDestructibleData(SavedData, GUID);
	if (SavedData)
	{
		HitsTaken = SavedData->HitsTaken;
		if (HitsTaken > 0)
		{
			UpdateAppearance();
			SetActorHiddenInGame(HitsTaken >= HitsToDestroy);
		}
	}
}

//~==============================================================================
// Save Game

void ADestructible::SubmitDataToBeSaved(FLevelSaveData& LevelSaveData) const
{
	FDestructibleSaveData DataToSave;
	DataToSave.HitsTaken = HitsTaken;
	LevelSaveData.Destructibles.Add(GUID, DataToSave);
}

