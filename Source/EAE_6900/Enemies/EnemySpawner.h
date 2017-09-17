// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// game includes
#include "StatsBuffs/StatEffect.h"

#include "EnemySpawner.generated.h"

// forward declarations
class ABasicEnemy;
class APathDataActor;
class USpawnerComponent;

UCLASS()
class EAE_6900_API AEnemySpawner : public AActor
{
	GENERATED_UCLASS_BODY()

	//~==============================================================================
	// Behavior
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnWaveStarted(int32 WaveIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnWaveEnded(int32 WaveIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnEnemyKilled(const FString& EnemyName, EStatKind Type);

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnEnemyTookDamage(const FString& EnemyName, float DamageAmount, EStatKind Type);

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnEnemyBlockedDamage(const FString& EnemyName, float DamageAmount, EStatKind Type);

protected:
	UFUNCTION()
	void OnEnemySpawned(AActor* SpawnedActor);

	UFUNCTION()
	void NotifyEnemyKilled(const FString& EnemyName, EStatKind Type);

	void NotifyWaveStarted(int32 WaveIndex);
	void NotifyWaveEnded(int32 WaveIndex);

	virtual void BeginPlay() override;

private:
	int32									LastWaveThatEnded = 0;
	int32									NumEnemiesAlive = 0;

	//~==============================================================================
	// Components
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USpawnerComponent*						SpawnerComponent = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawning")
	bool									bAutoStart = false;

public:
	FORCEINLINE USpawnerComponent* GetSpawnerComponent() const { return SpawnerComponent; }

}; // class AEnemySpawner
