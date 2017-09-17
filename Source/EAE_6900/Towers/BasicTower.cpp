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
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABasicTower::OnAttackTimerElapsed, FireRate, true);
}

void ABasicTower::OnAttackTimerElapsed()
{
	switch (Type)
	{
	case ETowerAttackType::Projectile:
		DoProjectileAttack();
		break;

	case ETowerAttackType::AOE:
		DoAOEAttack();
		break;
	}
}

void ABasicTower::DoProjectileAttack()
{
	TArray<AActor*> OverlappingActors;
	Sphere->GetOverlappingActors(OverlappingActors, ABasicEnemy::StaticClass());

	if (OverlappingActors.Num())
	{
		FVector TargetLocation;
		for (const auto& Target : OverlappingActors)
		{
			if (Target->IsPendingKillPending() == false)
			{
				TargetLocation = Target->GetActorLocation();
				break;
			}
		}

		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z += MuzzleLocation.Z;

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

void ABasicTower::DoAOEAttack()
{
	TArray<AActor*> OverlappingActors;
	Sphere->GetOverlappingActors(OverlappingActors, ABasicEnemy::StaticClass());

	if (OverlappingActors.Num())
	{
		for (const auto& ActorToDamage : OverlappingActors)
		{
			if (ActorToDamage->IsPendingKillPending())
			{
				continue;
			}

			FDamageEvent DamageEvent;
			DamageEvent.DamageTypeClass = StatEffectClass;

			ActorToDamage->TakeDamage(1.0f, DamageEvent, GetInstigatorController(), this);
		}
	}
}

