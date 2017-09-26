// Fill out your copyright notice in the Description page of Project Settings.

#include "EAE_6900Bullet.h"

// engine includes
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// game includes
#include "EAE_6900.h"
#include "EAE_6900Pawn.h"

// Sets default values
AEAE_6900Bullet::AEAE_6900Bullet()
{
    bReplicates = 1;
    bReplicateMovement = 1;

    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // setup collision
    SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
    SphereCollision->InitSphereRadius(10.0f);
    SphereCollision->BodyInstance.SetCollisionProfileName("Projectile");
    SphereCollision->OnComponentHit.AddDynamic(this, &AEAE_6900Bullet::EventOnComponentHit);

    SetRootComponent(SphereCollision);

    // setup static mesh
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetCollisionProfileName("NoCollision");
    StaticMesh->SetupAttachment(SphereCollision);

    // setup projectile movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = SphereCollision;
    ProjectileMovement->InitialSpeed = 4500.0f;
    ProjectileMovement->MaxSpeed = 5000.0f;
    ProjectileMovement->bShouldBounce = 0;
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    // self-destruction
    InitialLifeSpan = 0.5f;
}

// Called when the game starts or when spawned
void AEAE_6900Bullet::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AEAE_6900Bullet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEAE_6900Bullet::EventOnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (Role < ROLE_Authority)
    {
        // spawn effect
        if (ImpactFX)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, GetTransform(), true);
        }
    }
    else
    {
        // apply damage
        if (OtherActor && OtherActor->IsA(AEAE_6900Pawn::StaticClass()))
        {
            FDamageEvent DamageEvent;
            OtherActor->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
        }

        // die
        Destroy();
    }
}

