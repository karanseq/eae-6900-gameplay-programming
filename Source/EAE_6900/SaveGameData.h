#pragma once

// engine includes
#include "GameFramework/SaveGame.h"

// game includes
#include "EAE_6900.h"
#include "Collectible.h"

#include "SaveGameData.generated.h"

USTRUCT()
struct FManifestData
{
	GENERATED_USTRUCT_BODY()

	//~==============================================================================
	// Properties

	UPROPERTY(VisibleAnywhere, Category = SaveGame)
	int32											NumLevelSaveFiles = 0;

};

USTRUCT()
struct FDestructibleSaveData
{
	GENERATED_USTRUCT_BODY()

	//~==============================================================================
	// Properties

	UPROPERTY(VisibleAnywhere, Category = SaveGame)
	int32											HitsTaken = 0;

};

USTRUCT()
struct FCollectibleSaveData
{
	GENERATED_USTRUCT_BODY()

	//~==============================================================================
	// Properties

	UPROPERTY(VisibleAnywhere, Category = SaveGame)
	bool											bWasCollected = false;

};

USTRUCT()
struct FLevelSaveData
{
	GENERATED_USTRUCT_BODY()

	//~==============================================================================
	// Properties

	UPROPERTY(VisibleAnywhere, Category = SaveGame)
	FName											LevelName = NAME_None;

	UPROPERTY(VisibleAnywhere, Category = SaveGame)
	float											PlayerHealth = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = SaveGame)
	int32											PlayerAmmo = 0;

	UPROPERTY(VisibleAnywhere, Category = SaveGame)
	FTransform										PlayerTransform;

	UPROPERTY(VisibleAnywhere, Category = SaveGame)
	TMap<FName, FDestructibleSaveData>				Destructibles;

	UPROPERTY(VisibleAnywhere, Category = SaveGame)
	TMap<FName, FCollectibleSaveData>				Collectibles;

};

UCLASS()
class EAE_6900_API UEAE_6900ManifestSave : public USaveGame
{
	GENERATED_BODY()

	//~==============================================================================
	// Properties

public:
	UPROPERTY(VisibleAnywhere, Category = SaveGame)
	FManifestData									ManifestData;

};

UCLASS()
class EAE_6900_API UEAE_6900LevelSave : public USaveGame
{
	GENERATED_BODY()

	//~==============================================================================
	// Properties

public:
	UPROPERTY(VisibleAnywhere, Category = SaveGame)
	FLevelSaveData									LevelSaveData;

};