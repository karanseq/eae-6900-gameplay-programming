// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

// forward declarations
class ABasicEnemy;
class APathDataActor;

UCLASS()
class EAE_6900_API AEnemySpawner : public AActor
{
	GENERATED_UCLASS_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TArray<APathDataActor*>					PathDataActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TArray<TSubclassOf<ABasicEnemy>>		EnemiesToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta=(UIMin = "1.0", UIMax = "5.0"))
	float									SpawnRate = 2.5f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StartSpawning() const;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StopSpawning() const;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnEnemySpawned(const FString& EnemyName);

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnEnemyKilled(const FString& EnemyName);

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnEnemyMadeProgress(const FString& EnemyName, int32 Progress);

private:
	void SpawnAnActorAtRandom() const;

}; // class AEnemySpawner
