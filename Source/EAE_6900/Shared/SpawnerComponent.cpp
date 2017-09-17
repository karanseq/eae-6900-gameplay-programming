// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnerComponent.h"

// engine includes
#include "Kismet/GameplayStatics.h"

// game includes
#include "Enemies/BasicEnemy.h"
#include "Shared/PathDataActor.h"

// Sets default values for this component's properties
USpawnerComponent::USpawnerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoActivate = 1;
	PrimaryComponentTick.bCanEverTick = true;
}

void USpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState != ESpawnerState::None &&
		CurrentState != ESpawnerState::Idle)
	{
		UpdateTicker(DeltaTime);
	}
}

void USpawnerComponent::StartSpawning(int32 WaveIndex /* = 0*/, bool bIgnoreWaveStartDelay /* = false*/)
{
	if (CurrentState == ESpawnerState::Spawning ||
		CurrentState == ESpawnerState::WaitOnCurrentWave ||
		CurrentWaveIndex < 0 || CurrentWaveIndex >= WaveData->WaveList.Num())
	{
		return;
	}
	CurrentState = ESpawnerState::WaitOnCurrentWave;

	CurrentWaveIndex = WaveIndex;

	Ticker = bIgnoreWaveStartDelay ? 0.0f : GetDelayBeforeStartingCurrentWave();
}

void USpawnerComponent::StopSpawning()
{
	CurrentState = ESpawnerState::Idle;
}

void USpawnerComponent::UpdateTicker(float DeltaTime)
{
	Ticker -= DeltaTime;
	if (Ticker < 0)
	{
		switch (CurrentState)
		{
		case ESpawnerState::Spawning:
			SpawnPendingActorForCurrentWave();
			break;

		case ESpawnerState::WaitOnCurrentWave:
			StartCurrentWave();
			break;
		}
	}
}

void USpawnerComponent::GeneratePendingActorsForCurrentWave()
{
	if (CurrentWaveIndex < 0 || CurrentWaveIndex >= WaveData->WaveList.Num())
	{
		return;
	}

	const FSpawnableActorWave& CurrentWave = WaveData->WaveList[CurrentWaveIndex];

	struct FClassCountPair
	{
		TSubclassOf<ABasicEnemy>	Class;
		int32						Count;
	};
	TArray<FClassCountPair> ClassCountPairs;

	for (const auto& ActorData : CurrentWave.ActorDataList)
	{
		ClassCountPairs.Add({
			ActorData.ActorClass,
			FMath::RandRange(ActorData.MinCount, ActorData.MaxCount)
		});
	}

	uint8 Index = 0;
	while (ClassCountPairs.Num() > 0)
	{
		PendingActorsForCurrentWave.Add(ClassCountPairs[Index].Class);
		--ClassCountPairs[Index].Count;
		if (ClassCountPairs[Index].Count <= 0)
		{
			ClassCountPairs.RemoveAt(Index);
		}

		Index = Index + 1 >= ClassCountPairs.Num() ? 0 : Index + 1;
	}
}

void USpawnerComponent::SpawnPendingActorForCurrentWave()
{
	if (CurrentState != ESpawnerState::Spawning ||
		PendingActorsForCurrentWave.Num() <= 0)
	{
		return;
	}

	const TSubclassOf<ABasicEnemy>& PendingActorClass = PendingActorsForCurrentWave[0];

	for (const auto& PathData : PathDataList)
	{
		const FTransform SpawnTransform(PathData->GetDirection(0.0f), PathData->GetLocation(0.0f));

		if (ABasicEnemy* SpawnedActor = GetWorld()->SpawnActorDeferred<ABasicEnemy>(PendingActorClass, SpawnTransform))
		{
			SpawnedActor->SetPathDataActor(PathData);
			UGameplayStatics::FinishSpawningActor(SpawnedActor, SpawnTransform);

			OnActorSpawned.Broadcast(SpawnedActor);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Couldn't spawn actor of type : %s!"), *PendingActorClass->GetName());
		}
	}

	PendingActorsForCurrentWave.RemoveAt(0);
	if (PendingActorsForCurrentWave.Num() > 0)
	{
		Ticker = GetRandomSpawnDelayForCurrentWave();
	}
	else
	{
		OnFinishedSpawningPendingActors();
	}
}

void USpawnerComponent::OnFinishedSpawningPendingActors()
{
	if (CurrentState != ESpawnerState::Spawning)
	{
		return;
	}
	CurrentState = ESpawnerState::WaitOnCurrentWave;

	OnWaveEnded.Broadcast(CurrentWaveIndex);

	++CurrentWaveIndex;

	// check whether the previous wave was the last one
	if (CurrentWaveIndex < WaveData->WaveList.Num())
	{
		// if not start waiting for the next one to start
		Ticker = GetDelayBeforeStartingCurrentWave();
	}
	else
	{
		// we're done
		CurrentWaveIndex = 0;
		CurrentState = ESpawnerState::Idle;

		// if we're looping, go back to waiting on the first wave
		if (bLoopWaves)
		{
			CurrentState = ESpawnerState::WaitOnCurrentWave;
		}
	}

}

void USpawnerComponent::StartCurrentWave()
{
	if (CurrentState != ESpawnerState::WaitOnCurrentWave)
	{
		return;
	}
	CurrentState = ESpawnerState::Spawning;

	OnWaveStarted.Broadcast(CurrentWaveIndex);

	GeneratePendingActorsForCurrentWave();
	Ticker = GetRandomSpawnDelayForCurrentWave();
}

float USpawnerComponent::GetDelayBeforeStartingCurrentWave() const
{
	if (CurrentWaveIndex < 0 || CurrentWaveIndex >= WaveData->WaveList.Num())
	{
		return 0.0f;
	}
	return WaveData->WaveList[CurrentWaveIndex].DelayBeforeStart;
}

float USpawnerComponent::GetRandomSpawnDelayForCurrentWave() const
{
	if (CurrentWaveIndex < 0 || CurrentWaveIndex >= WaveData->WaveList.Num())
	{
		return 0.0f;
	}
	return FMath::RandRange(WaveData->WaveList[CurrentWaveIndex].MinDelayBetweenSpawns, WaveData->WaveList[CurrentWaveIndex].MaxDelayBetweenSpawns);
}

