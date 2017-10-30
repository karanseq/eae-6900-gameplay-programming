#pragma once

// engine includes
#include "GameFramework/SaveGame.h"

// game includes
#include "EAE_6900.h"

#include "SaveGameData.generated.h"

USTRUCT(BlueprintType)
struct FManifestData
{
	GENERATED_USTRUCT_BODY()

	//~==============================================================================
	// Properties

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SaveGame)
	TArray<FDateTime>								LevelTimestampList;

};

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_USTRUCT_BODY()

	//~==============================================================================
	// Properties

	UPROPERTY(VisibleAnywhere, Category = SaveGame)
	float											PlayerHealth = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = SaveGame)
	int32											PlayerAmmo = 0;

	UPROPERTY(VisibleAnywhere, Category = SaveGame)
	FTransform										PlayerTransform;

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
	FPlayerSaveData									PlayerSaveData;

	UPROPERTY(VisibleAnywhere, Category = SaveGame)
	TMap<FName, FDestructibleSaveData>				Destructibles;

	UPROPERTY(VisibleAnywhere, Category = SaveGame)
	TMap<FName, FCollectibleSaveData>				Collectibles;

};

class EAE_6900_API ISaveable
{
public:
	virtual ~ISaveable() = default;

	virtual void SubmitDataToBeSaved(FLevelSaveData& LevelSaveData) const = 0;
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