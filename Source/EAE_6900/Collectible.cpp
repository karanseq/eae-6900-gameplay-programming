// Fill out your copyright notice in the Description page of Project Settings.

#include "Collectible.h"

// engine includes
#include "Components/BoxComponent.h"

// game includes
#include "EAE_6900GameInstance.h"
#include "EAE_6900Pawn.h"

ACollectible::ACollectible(const FObjectInitializer& ObjectInitializer)
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

void ACollectible::Collected(bool bWithAnimation /* = true */)
{
	bCollected = true;
	BoxComponent->SetCollisionProfileName(FName(TEXT("NoCollision")));
	SetActorHiddenInGame(true);
}

void ACollectible::BeginPlay()
{
	ensure(GUID != NAME_None);

	UEAE_6900GameInstance* GameInstance = UEAE_6900GameInstance::GetInstance();
	GameInstance->RegisterSaveableObject(this);

	Super::BeginPlay();

	const FCollectibleSaveData* SavedData = nullptr;
	GameInstance->GetCurrentlyLoadedCollectibleData(SavedData, GUID);
	if (SavedData)
	{
		if (SavedData->bWasCollected)
		{
			Collected(/*bWithAnimation =*/ false);
		}
	}
}

//~==============================================================================
// Save Game

void ACollectible::SubmitDataToBeSaved(FLevelSaveData& LevelSaveData) const
{
	FCollectibleSaveData DataToSave;
	DataToSave.bWasCollected = bCollected;
	LevelSaveData.Collectibles.Add(GUID, DataToSave);
}

