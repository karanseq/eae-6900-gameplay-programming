// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicEnemy.h"

// engine includes
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"

// game includes
#include "Shared/PathDataActor.h"

// Sets default values
ABasicEnemy::ABasicEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = Box;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ABasicEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	OnEnemyKilled.ExecuteIfBound(GetName());
}

// Called every frame
void ABasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PathDataActor)
	{
		AccumulatedTime += (DeltaTime * MovementSpeed);

		FHitResult HitResult;
		SetActorLocationAndRotation(PathDataActor->GetLocation(AccumulatedTime),
			PathDataActor->GetDirection(AccumulatedTime),
			false,
			&HitResult,
			ETeleportType::None
		);

		if (AccumulatedTime > PathDataActor->GetDuration())
		{
			PathDataActor = nullptr;
			Destroy();
		}
	}
}

float ABasicEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	UStatEffect const * const StatEffectCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UStatEffect>() : GetDefault<UStatEffect>();

	const float PreviousHealth = CurrentHealth;

	switch (StatEffectCDO->Stat)
	{
	case EStatKind::Physical:
		CurrentHealth -= FMath::FloorToFloat(FMath::RandRange(StatEffectCDO->Min, StatEffectCDO->Max));
		break;

	case EStatKind::Magic:
		CurrentHealth -= FMath::FloorToFloat(FMath::RandRange(StatEffectCDO->Min, StatEffectCDO->Max));
		break;

	case EStatKind::Slow:
		break;

	case EStatKind::Stun:
		break;
	}

	OnEnemyTookDamage.ExecuteIfBound(GetName(), PreviousHealth - CurrentHealth, StatEffectCDO->Stat);

	if (CurrentHealth <= 0.0f)
	{
		Destroy();
	}

	return PreviousHealth - CurrentHealth;
}

