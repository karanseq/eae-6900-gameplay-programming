// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"

#include "EAE_6900Bullet.generated.h"

UCLASS()
class EAE_6900_API AEAE_6900Bullet : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AEAE_6900Bullet();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(VisibleAnywhere)
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(VisibleDefaultsOnly)
    USphereComponent* SphereCollision;

    UPROPERTY(EditAnywhere, Category = Stats)
    float Damage = 5.0f;

    UPROPERTY(EditAnywhere, Category = Visuals)
    UParticleSystem* ImpactFX;

public:
    UFUNCTION()
    void EventOnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
