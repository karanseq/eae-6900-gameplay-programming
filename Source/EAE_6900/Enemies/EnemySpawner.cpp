// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"

// engine includes
#include "Kismet/GameplayStatics.h"

// game includes
#include "Enemies/BasicEnemy.h"
#include "Shared/PathDataActor.h"
#include "Shared/SpawnerComponent.h"

// Sets default values
AEnemySpawner::AEnemySpawner(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnerComponent = CreateDefaultSubobject<USpawnerComponent>(TEXT("SpawnerComponent"));
	SpawnerComponent->GetOnActorSpawned().AddDynamic(this, &AEnemySpawner::OnEnemySpawned);
	SpawnerComponent->GetOnWaveStarted().AddDynamic(this, &AEnemySpawner::OnWaveStarted);
	SpawnerComponent->GetOnWaveEnded().AddDynamic(this, &AEnemySpawner::OnWaveEnded);
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	//SpawnerComponent->StartSpawning();
}

void AEnemySpawner::OnEnemySpawned(AActor* SpawnedActor)
{
	ABasicEnemy* SpawnedEnemy = Cast<ABasicEnemy>(SpawnedActor);
	SpawnedEnemy->GetEnemyKilledEvent().BindUObject(this, &AEnemySpawner::OnEnemyKilled);
	SpawnedEnemy->GetEnemyTookDamageEvent().BindUObject(this, &AEnemySpawner::OnEnemyTookDamage);
	SpawnedEnemy->GetEnemyBlockedDamageEvent().BindUObject(this, &AEnemySpawner::OnEnemyBlockedDamage);
}
