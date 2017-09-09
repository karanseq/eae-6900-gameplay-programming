// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"

// engine includes
#include "Kismet/GameplayStatics.h"

// game includes
#include "BasicEnemy.h"
#include "Shared/PathDataActor.h"

// Sets default values
AEnemySpawner::AEnemySpawner(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::StartSpawning() const
{
	if (EnemiesToSpawn.Num() > 0 &&
		PathDataActors.Num() > 0)
	{
		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &AEnemySpawner::SpawnAnActorAtRandom, SpawnRate, true);
	}
}

void AEnemySpawner::StopSpawning() const
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void AEnemySpawner::SpawnAnActorAtRandom() const
{
	// get parameters ready for the new actor
	UClass* RandomClass = EnemiesToSpawn[FMath::RandRange(0, EnemiesToSpawn.Num() - 1)];
	APathDataActor* RandomSpline = PathDataActors[FMath::RandRange(0, PathDataActors.Num() - 1)];
	const FTransform SpawnTransform(RandomSpline->GetDirection(0.0f), RandomSpline->GetLocation(0.0f));

	ABasicEnemy* SpawnedActor = GetWorld()->SpawnActorDeferred<ABasicEnemy>(RandomClass, SpawnTransform);

	SpawnedActor->SetPathDataActor(RandomSpline);
	SpawnedActor->GetEnemyKilledEvent().BindUObject(this, &AEnemySpawner::OnEnemyKilled);
	SpawnedActor->GetEnemyTookDamageEvent().BindUObject(this, &AEnemySpawner::OnEnemyTookDamage);

	UGameplayStatics::FinishSpawningActor(SpawnedActor, SpawnTransform);
}

