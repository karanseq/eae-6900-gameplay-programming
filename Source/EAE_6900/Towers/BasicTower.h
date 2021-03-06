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

UENUM(BlueprintType)
enum class ETowerAttackType : uint8
{
	None						UMETA(DisplayName = "Invalid"),
	Projectile					UMETA(DisplayName = "Projectile"),
	AOE							UMETA(DisplayName = "Area of Effect")
};

UCLASS(BlueprintType)
class EAE_6900_API ABasicTower : public APawn
{
	GENERATED_UCLASS_BODY()

	//~==============================================================================
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent*							Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent*						Mesh;

	//~==============================================================================
	// Behavior
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behavior")
	ETowerAttackType							Type = ETowerAttackType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior", meta = (UIMin = "0.1"))
	float										MinFireRate = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior", meta = (UIMin = "0.1"))
	float										MaxFireRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behavior")
	TSubclassOf<UStatEffect>					StatEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behavior")
	TSubclassOf<ABasicProjectile>				ProjectileBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior", meta=(MakeEditWidget = true))
	FVector										MuzzleLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void NotifyOnGameOver();

	virtual void Tick(float DeltaTime) override;

private:
	void OnAttackTimerElapsed();
	void DoProjectileAttack();
	void DoAOEAttack();

private:
	bool										bIsGameOver = false;
	float										Ticker = 0.0f;

};
