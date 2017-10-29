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
	void SaveLevel() const;

private:
	void GetLevelSaveSlotName(FString& OutLevelSaveFileName, const int32 Index) const;

public:
	static const FString ManifestSlotName;
	static const FString LevelSlotPrefix;

private:
	FManifestData ManifestData;
	FLevelSaveData CurrentlyLoadedLevelData;

private:
	void Init() override;
	void Shutdown() override;

};
