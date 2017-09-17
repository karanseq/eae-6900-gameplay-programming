// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"

// engine includes
#include "Kismet/GameplayStatics.h"

// game includes
#include "Enemies/BasicEnemy.h"
#include "Shared/PathDataActor.h"
#include "Shared/SpawnerComponent.h"
#include "Shared/TDPlayerController.h"

// Sets default values
AEnemySpawner::AEnemySpawner(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnerComponent = CreateDefaultSubobject<USpawnerComponent>(TEXT("SpawnerComponent"));
	SpawnerComponent->GetOnActorSpawned().AddUObject(this, &AEnemySpawner::OnEnemySpawned);
	SpawnerComponent->GetOnWaveStarted().AddUObject(this, &AEnemySpawner::NotifyWaveStarted);
	SpawnerComponent->GetOnWaveEnded().AddUObject(this, &AEnemySpawner::NotifyWaveEnded);
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemySpawner::OnEnemySpawned(AActor* SpawnedActor)
{
	ABasicEnemy* SpawnedEnemy = Cast<ABasicEnemy>(SpawnedActor);
	SpawnedEnemy->GetEnemyKilledEvent().AddUObject(this, &AEnemySpawner::NotifyEnemyKilled);
	SpawnedEnemy->GetEnemyTookDamageEvent().AddUObject(this, &AEnemySpawner::OnEnemyTookDamage);
	SpawnedEnemy->GetEnemyBlockedDamageEvent().AddUObject(this, &AEnemySpawner::OnEnemyBlockedDamage);

	++NumEnemiesAlive;
}

void AEnemySpawner::NotifyEnemyKilled(const FString& EnemyName, EStatKind Type)
{
	OnEnemyKilled(EnemyName, Type);

	--NumEnemiesAlive;
	if (LastWaveThatEnded >= SpawnerComponent->GetWaveData()->WaveList.Num() - 1 &&
		NumEnemiesAlive <= 0)
	{
		ATDPlayerController* Player = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (Player)
		{
			Player->OnGameWin();
		}
	}
}

void AEnemySpawner::NotifyWaveStarted(int32 WaveIndex)
{
	OnWaveStarted(WaveIndex);
}

void AEnemySpawner::NotifyWaveEnded(int32 WaveIndex)
{
	LastWaveThatEnded = WaveIndex;
	OnWaveEnded(WaveIndex);
}
