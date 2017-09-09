// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicProjectile.h"

// engine includes
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// game includes
#include "StatsBuffs/StatEffect.h"

// Sets default values
ABasicProjectile::ABasicProjectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 0.2f;
	bCanBeDamaged = 0;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	RootComponent = Sphere;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->bAutoActivate = 1;
	ProjectileMovement->InitialSpeed = 50000.0f;
	ProjectileMovement->MaxSpeed = 50000.0f;
	ProjectileMovement->bRotationFollowsVelocity = 1;
	ProjectileMovement->bShouldBounce = 0;
	ProjectileMovement->bIsHomingProjectile = 0;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void ABasicProjectile::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	FDamageEvent DamageEvent;
	DamageEvent.DamageTypeClass = StatEffectClass;

	Other->TakeDamage(1.0f, DamageEvent, GetInstigatorController(), this);

	Destroy();
}
