// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

// game includes
#include "StatsBuffs/StatEffect.h"

#include "BasicEnemy.generated.h"

// forward declarations
class APathDataActor;
class UBoxComponent;
class UStaticMeshComponent;

// delegate declarations
DECLARE_DELEGATE_OneParam(FEnemyKilledEvent, const FString&);
DECLARE_DELEGATE_ThreeParams(FEnemyTookDamageEvent, const FString&, float, EStatKind);

UCLASS()
class EAE_6900_API ABasicEnemy : public APawn
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
	UBoxComponent*								Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
	UStaticMeshComponent*						Mesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE void SetPathDataActor(APathDataActor* PathDataActorIn) { PathDataActor = PathDataActorIn; }
	FORCEINLINE FEnemyKilledEvent& GetEnemyKilledEvent() { return OnEnemyKilled; }
	FORCEINLINE FEnemyTookDamageEvent& GetEnemyTookDamageEvent() { return OnEnemyTookDamage; }

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float										MaxHealth = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float										CurrentHealth = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float										MovementSpeed = 1.0f;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	APathDataActor*								PathDataActor = nullptr;
	float										AccumulatedTime = 0.0f;

	FEnemyKilledEvent							OnEnemyKilled;
	FEnemyTookDamageEvent						OnEnemyTookDamage;
};
