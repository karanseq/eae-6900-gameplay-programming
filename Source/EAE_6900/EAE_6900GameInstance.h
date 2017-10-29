// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

// game includes
#include "EAE_6900.h"
#include "SaveGameData.h"

#include "EAE_6900GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EAE_6900_API UEAE_6900GameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()
	
public:
	FORCEINLINE static UEAE_6900GameInstance* GetInstance() { return Instance; }

private:
	// A static instance of this class for easy access
	static UEAE_6900GameInstance*					Instance;

	//~==============================================================================
	// Game Loop
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game Loop")
	bool BeginPlay();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game Loop")
	bool EndPlay(EEndPlayReason::Type EndPlayReasonIn);

	//~==============================================================================
	// Save Game
public:
	UFUNCTION(BlueprintCallable, Category = "Game Loop")
	void LoadManifest();
	UFUNCTION(BlueprintCallable, Category = "Game Loop")
	void SaveManifest() const;
	UFUNCTION(BlueprintCallable, Category = "Game Loop")
	void LoadLevel(int32 Index);
	UFUNCTION(BlueprintCallable, Category = "Game Loop")
	void SaveLevel();

	FORCEINLINE void RegisterSaveableObject(ISaveable* SaveableObject) { SaveableObjectList.AddUnique(SaveableObject); }
	FORCEINLINE void GetCurrentlyLoadedPlayerData(const FPlayerSaveData*& LevelSaveData) const { LevelSaveData = bCurrentLevelDataExists ? &CurrentlyLoadedLevelData.PlayerSaveData : nullptr; }
	FORCEINLINE void GetCurrentlyLoadedDestructibleData(const FDestructibleSaveData*& DestructibleSaveData, const FName& GUID) const { DestructibleSaveData = bCurrentLevelDataExists ? CurrentlyLoadedLevelData.Destructibles.Find(GUID) : nullptr; }
	FORCEINLINE void GetCurrentlyLoadedCollectibleData(const FCollectibleSaveData*& CollectibleSaveData, const FName& GUID) const { CollectibleSaveData = bCurrentLevelDataExists ? CurrentlyLoadedLevelData.Collectibles.Find(GUID) : nullptr; }

private:
	void GetLevelSaveSlotName(FString& OutLevelSaveFileName, const int32 Index) const;

public:
	static const FString ManifestSlotName;
	static const FString LevelSlotPrefix;

private:
	FManifestData ManifestData;
	TArray<ISaveable*> SaveableObjectList;

	bool bCurrentLevelDataExists = false;
	FLevelSaveData CurrentlyLoadedLevelData;

private:
	void Init() override;
	void Shutdown() override;

};
