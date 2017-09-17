// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WaveData.generated.h"

// forward declarations
class ABasicEnemy;

UENUM(BlueprintType)
enum class ESpawnerState : uint8
{
	None						UMETA(DisplayName = "Invalid State"),
	Idle						UMETA(DisplayName = "Idle"),
	Spawning					UMETA(DisplayName = "Spawning"),
	WaitOnCurrentWave			UMETA(DisplayName = "Waiting to start current wave"),
};

USTRUCT(BlueprintType, meta = (DisplayName = "Actor Data"))
struct FSpawnableActorData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ABasicEnemy>			ActorClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta=(UIMin = "0"))
	int32								MinCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (UIMin = "0"))
	int32								MaxCount = 0;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Wave"))
struct FSpawnableActorWave
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<FSpawnableActorData>			ActorDataList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (UIMin = "0.0"))
	float								MinDelayBetweenSpawns = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (UIMin = "0.0"))
	float								MaxDelayBetweenSpawns = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (UIMin = "0.0"))
	float								DelayBeforeStart = 0.0f;
};

UCLASS()
class EAE_6900_API UWaveData : public UDataAsset
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
	TArray<FSpawnableActorWave>			WaveList;
	
};
