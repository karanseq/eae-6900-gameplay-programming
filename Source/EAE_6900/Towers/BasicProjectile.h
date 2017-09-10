// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicProjectile.generated.h"

// forward declarations
class UProjectileMovementComponent;
class USphereComponent;
class UStaticMeshComponent;
class UStatEffect;

UCLASS(BlueprintType)
class EAE_6900_API ABasicProjectile : public AActor
{
	GENERATED_UCLASS_BODY()

	//~==============================================================================
	// Components
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* Sphere = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	//~==============================================================================
	// Behavior
public:
	UPROPERTY(BlueprintReadOnly, Category = Weapon)
	TSubclassOf<UStatEffect> StatEffectClass = nullptr;

private:
	void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
};
