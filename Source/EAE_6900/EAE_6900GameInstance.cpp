// Fill out your copyright notice in the Description page of Project Settings.

#include "EAE_6900GameInstance.h"

// engine includes
#include "Kismet/GameplayStatics.h"

// game includes

// static member initialization
UEAE_6900GameInstance* UEAE_6900GameInstance::Instance = nullptr;
const FString UEAE_6900GameInstance::ManifestSlotName(TEXT("EAE_6900_MAN"));
const FString UEAE_6900GameInstance::LevelSlotPrefix(TEXT("EAE_6900_LVL_"));

UEAE_6900GameInstance::UEAE_6900GameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UEAE_6900GameInstance::Init()
{
	UE_LOG(LogGame, Log, TEXT("UEAE_6900GameInstance::Init!"));
	UEAE_6900GameInstance::Instance = this;
}

void UEAE_6900GameInstance::Shutdown()
{
	UEAE_6900GameInstance::Instance = nullptr;
	UE_LOG(LogGame, Log, TEXT("UEAE_6900GameInstance::Shutdown!"));
}

bool UEAE_6900GameInstance::BeginPlay_Implementation()
{
	return true;
}

bool UEAE_6900GameInstance::EndPlay_Implementation(EEndPlayReason::Type EndPlayReasonIn)
{
	return true;
}

void UEAE_6900GameInstance::LoadManifest()
{
	if (UGameplayStatics::DoesSaveGameExist(ManifestSlotName, 0) == false)
	{
		SaveManifest();
		return;
	}

	if (UEAE_6900ManifestSave* LoadManifestInstance = Cast<UEAE_6900ManifestSave>(UGameplayStatics::LoadGameFromSlot(ManifestSlotName, 0)))
	{
		ManifestData.NumLevelSaveFiles = LoadManifestInstance->ManifestData.NumLevelSaveFiles;
	}
	else
	{
		UE_LOG(LogGame, Error, TEXT("Couldn't load manifest!"));
	}
}

void UEAE_6900GameInstance::SaveManifest() const
{
	if (UEAE_6900ManifestSave* ManifestSaveInstance = Cast<UEAE_6900ManifestSave>(UGameplayStatics::CreateSaveGameObject(UEAE_6900ManifestSave::StaticClass())))
	{
		ManifestSaveInstance->ManifestData.NumLevelSaveFiles = ManifestData.NumLevelSaveFiles;

		UGameplayStatics::SaveGameToSlot(ManifestSaveInstance, ManifestSlotName, 0);
		UE_LOG(LogGame, Log, TEXT("Manifest saved successfully!"));
	}
	else
	{
		UE_LOG(LogGame, Error, TEXT("Couldn't save manifest!"));
	}
}

void UEAE_6900GameInstance::LoadLevel(int32 Index)
{
	ensure(Index >= 0 && Index < ManifestData.NumLevelSaveFiles);

	FString LevelSaveSlotName;
	GetLevelSaveSlotName(LevelSaveSlotName, Index);

	if (UGameplayStatics::DoesSaveGameExist(LevelSaveSlotName, 0) == false)
	{
		UE_LOG(LogGame, Error, TEXT("Couldn't find level save file for index:%d"), Index);
		return;
	}

	if (UEAE_6900LevelSave* LoadLevelSaveInstance = Cast<UEAE_6900LevelSave>(UGameplayStatics::LoadGameFromSlot(LevelSaveSlotName, 0)))
	{
		CurrentlyLoadedLevelData.LevelName = LoadLevelSaveInstance->LevelSaveData.LevelName;
		CurrentlyLoadedLevelData.PlayerHealth = LoadLevelSaveInstance->LevelSaveData.PlayerHealth;
		CurrentlyLoadedLevelData.PlayerAmmo = LoadLevelSaveInstance->LevelSaveData.PlayerAmmo;
		CurrentlyLoadedLevelData.PlayerTransform = LoadLevelSaveInstance->LevelSaveData.PlayerTransform;

		for (const auto& DestructibleSaveData : LoadLevelSaveInstance->LevelSaveData.Destructibles)
		{
			CurrentlyLoadedLevelData.Destructibles.Add(DestructibleSaveData.Key, DestructibleSaveData.Value);
		}

		for (const auto& CollectibleSaveData : LoadLevelSaveInstance->LevelSaveData.Collectibles)
		{
			CurrentlyLoadedLevelData.Collectibles.Add(CollectibleSaveData.Key, CollectibleSaveData.Value);
		}
	}
	else
	{
		UE_LOG(LogGame, Error, TEXT("Couldn't load level save file for index:%d"), Index);
	}
}

void UEAE_6900GameInstance::SaveLevel() const
{
	if (UEAE_6900LevelSave* LevelSaveInstance = Cast<UEAE_6900LevelSave>(UGameplayStatics::CreateSaveGameObject(UEAE_6900LevelSave::StaticClass())))
	{
		// copy all properties into the save game object
		{
			LevelSaveInstance->LevelSaveData.LevelName = CurrentlyLoadedLevelData.LevelName;
			LevelSaveInstance->LevelSaveData.PlayerHealth = CurrentlyLoadedLevelData.PlayerHealth;
			LevelSaveInstance->LevelSaveData.PlayerAmmo = CurrentlyLoadedLevelData.PlayerAmmo;
			LevelSaveInstance->LevelSaveData.PlayerTransform = CurrentlyLoadedLevelData.PlayerTransform;

			for (const auto& DestructibleSaveData : CurrentlyLoadedLevelData.Destructibles)
			{
				LevelSaveInstance->LevelSaveData.Destructibles.Add(DestructibleSaveData.Key, DestructibleSaveData.Value);
			}

			for (const auto& CollectibleSaveData : CurrentlyLoadedLevelData.Collectibles)
			{
				LevelSaveInstance->LevelSaveData.Collectibles.Add(CollectibleSaveData.Key, CollectibleSaveData.Value);
			}
		}

		// get a new slot name for this save file
		FString LevelSaveSlotName;
		GetLevelSaveSlotName(LevelSaveSlotName, ManifestData.NumLevelSaveFiles);

		UGameplayStatics::SaveGameToSlot(LevelSaveInstance, LevelSaveSlotName, 0);
		UE_LOG(LogGame, Log, TEXT("Level saved successfully!"));
	}
	else
	{
		UE_LOG(LogGame, Error, TEXT("Couldn't save level!"));
	}
}

void UEAE_6900GameInstance::GetLevelSaveSlotName(FString& OutLevelSaveSlotName, const int32 Index) const
{
	OutLevelSaveSlotName = FString::Printf(TEXT("%s%02d"), *LevelSlotPrefix, Index);
}
