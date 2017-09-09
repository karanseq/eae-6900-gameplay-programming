// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

// game includes
#include "StatsBuffs/StatEffect.h"

#include "BasicTower.generated.h"

// forward declarations
class ABasicProjectile;
class USphereComponent;
class UStatEffect;
class UStaticMeshComponent;

UCLASS(BlueprintType)
class EAE_6900_API ABasicTower : public APawn
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
	USphereComponent*							Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
	UStaticMeshComponent*						Mesh;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
	float										FireRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behavior")
	TSubclassOf<UStatEffect>					StatEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behavior")
	TSubclassOf<ABasicProjectile>				ProjectileBP;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Behavior", meta=(MakeEditWidget = true))
	FVector										MuzzleLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void FireProjectile();

private:
	FTimerHandle								FireTimerHandle;

};
