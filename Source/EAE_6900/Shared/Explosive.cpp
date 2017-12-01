// Fill out your copyright notice in the Description page of Project Settings.

#include "Explosive.h"

// engine includes
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// game includes
#include "AI/Shared/EAE_6900EnemyCharacter.h"
#include "Player/EAE_6900PlayerCharacter.h"

// Sets default values
AExplosive::AExplosive(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetCollisionProfileName(FName("OverlapAllDynamic"));
	RootComponent = SphereCollision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(FName("BlockAll"));
	Mesh->SetupAttachment(RootComponent);
}

//~==============================================================================
// Behavior

void AExplosive::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	constexpr float ExplosionRadiusMultiplier = 2;
	Radius = SphereCollision->GetScaledSphereRadius() * ExplosionRadiusMultiplier;
}

void AExplosive::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if ((TargetType == EExplosiveTargetType::Player &&
		OtherActor->IsA(AEAE_6900PlayerCharacter::StaticClass())) ||
		(TargetType == EExplosiveTargetType::Enemy &&
		OtherActor->IsA(AEAE_6900EnemyCharacter::StaticClass())))
	{
		FRadialDamageEvent RadialDamage;
		RadialDamage.DamageTypeClass = UDamageType::StaticClass();
		RadialDamage.Origin = GetActorLocation();
		RadialDamage.Params = FRadialDamageParams(BaseDamage, MinDamage, /*InInnerRadius = */ 0.0f, Radius, /*InDamageFallOff = */ 0.75f);

		OtherActor->TakeDamage(BaseDamage, RadialDamage, GetInstigatorController(), this);

		DoExplosionEffects();
		Destroy();
	}
}
