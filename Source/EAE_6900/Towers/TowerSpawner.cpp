// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerSpawner.h"

// engine includes
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// game includes
#include "Shared/TDPlayerController.h"
#include "Towers/BasicTower.h"

// Sets default values
ATowerSpawner::ATowerSpawner(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetCollisionProfileName(TEXT("UI"));
	Mesh->SetupAttachment(RootComponent);
}

void ATowerSpawner::NotifyActorOnClicked(FKey ButtonPressed /* = EKeys::LeftMouseButton*/)
{
	ATDPlayerController* Player = Cast<ATDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (Player && Player->GetIsGameOver())
	{
		return;
	}

	ShowTowerSelection();
}

void ATowerSpawner::OnPhysicalTowerSelected()
{
	SpawnTower(PhysicalTowerBP);
	Destroy();
}

void ATowerSpawner::OnMagicalTowerSelected()
{
	SpawnTower(MagicalTowerBP);
	Destroy();
}

void ATowerSpawner::OnSlowingTowerSelected()
{
	SpawnTower(SlowingTowerBP);
	Destroy();
}

void ATowerSpawner::SpawnTower(const TSubclassOf<ABasicTower>& TowerClass)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.Instigator = Instigator;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ABasicTower* Tower = GetWorld()->SpawnActor<ABasicTower>(TowerClass, GetActorTransform(), SpawnParams);
	if (Tower == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn tower!"));
	}
}
