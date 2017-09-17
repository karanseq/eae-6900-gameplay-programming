// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// game includes
#include "Shared/WaveData.h"

#include "SpawnerComponent.generated.h"

// forward declarations
class APathDataActor;

// dynamic multicast delegates for the spawner component (Blueprintable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorSpawnedSignature, AActor*, SpawnedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaveEventSignature, int32, WaveIndex);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EAE_6900_API USpawnerComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	//~==============================================================================
	// Wave Data
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UWaveData*							WaveData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TArray<APathDataActor*>				PathDataList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	bool								bLoopWaves = false;

	UPROPERTY(BlueprintReadOnly, Category = "Spawning")
	int32								CurrentWaveIndex = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Spawning")
	ESpawnerState						CurrentState = ESpawnerState::Idle;

public:
	FORCEINLINE UWaveData* GetWaveData() const { return WaveData; }
	FORCEINLINE const TArray<APathDataActor*>& GetPathDataList() const { return PathDataList; }
	FORCEINLINE int32 GetCurrentWaveIndex() const { return CurrentWaveIndex; }
	FORCEINLINE ESpawnerState GetCurrentState() const { return CurrentState; }

	//~==============================================================================
	// Behavior
public:
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StartSpawning(bool bReset = true);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StopSpawning();

	FORCEINLINE FActorSpawnedSignature& GetOnActorSpawned() { return OnActorSpawned; }
	FORCEINLINE FWaveEventSignature& GetOnWaveStarted() { return OnWaveStarted; }
	FORCEINLINE FWaveEventSignature& GetOnWaveEnded() { return OnWaveEnded; }

protected:
	UPROPERTY(BlueprintAssignable, Category = "Spawning")
	FActorSpawnedSignature				OnActorSpawned;

	UPROPERTY(BlueprintAssignable, Category = "Spawning")
	FWaveEventSignature					OnWaveStarted;

	UPROPERTY(BlueprintAssignable, Category = "Spawning")
	FWaveEventSignature					OnWaveEnded;

private:
	TArray<TSubclassOf<ABasicEnemy>>	PendingActorsForCurrentWave;
	float								Ticker;

private:
	void UpdateTicker(float DeltaTime);

	void StartCurrentWave();

	void GeneratePendingActorsForCurrentWave();
	void SpawnPendingActorForCurrentWave();
	void OnFinishedSpawningPendingActors();

	float GetDelayBeforeStartingCurrentWave() const;
	float GetRandomSpawnDelayForCurrentWave() const;

	//~==============================================================================
	// Actor Component
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
	
};
