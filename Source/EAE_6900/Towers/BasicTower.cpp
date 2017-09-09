// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicTower.h"

// engine includes
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// game includes
#include "Enemies/BasicEnemy.h"
#include "Towers/BasicProjectile.h"

// Sets default values
ABasicTower::ABasicTower(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = Sphere;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABasicTower::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ABasicTower::FireProjectile, FireRate, true);
}

void ABasicTower::FireProjectile()
{
	TArray<AActor*> OverlappingActors;
	Sphere->GetOverlappingActors(OverlappingActors, ABasicEnemy::StaticClass());

	if (OverlappingActors.Num())
	{
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z += MuzzleLocation.Z;
		FVector TargetLocation = OverlappingActors[0]->GetActorLocation();

		FTransform ProjectileTransform;
		ProjectileTransform.SetTranslation(SpawnLocation);
		ProjectileTransform.SetRotation(FQuat((TargetLocation - SpawnLocation).Rotation()));

		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;
		SpawnParams.Instigator = Instigator;

		ABasicProjectile* Projectile = GetWorld()->SpawnActor<ABasicProjectile>(ProjectileBP, ProjectileTransform, SpawnParams);
		if (Projectile)
		{
			Projectile->StatEffectClass = StatEffectClass;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn projectile!"));
		}

	}
}

