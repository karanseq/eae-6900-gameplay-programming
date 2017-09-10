// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

// game includes
#include "StatsBuffs/Buff.h"
#include "StatsBuffs/BuffInterface.h"
#include "StatsBuffs/StatEffect.h"

#include "BasicEnemy.generated.h"

// forward declarations
class APathDataActor;
class UBoxComponent;
class UStaticMeshComponent;

// delegate declarations
DECLARE_DELEGATE_TwoParams(FEnemyKilledEvent, const FString&, EStatKind);
DECLARE_DELEGATE_ThreeParams(FEnemyDamageEvent, const FString&, float, EStatKind);

UCLASS()
class EAE_6900_API ABasicEnemy : public APawn, public IBuffInterface
{
	GENERATED_UCLASS_BODY()

	//~==============================================================================
	// Components
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent*								Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent*						Mesh;

	//~==============================================================================
	// Movement Behavior
public:
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE void SetPathDataActor(APathDataActor* PathDataActorIn) { PathDataActor = PathDataActorIn; }

private:
	APathDataActor*								PathDataActor = nullptr;
	float										AccumulatedTime = 0.0f;

	//~==============================================================================
	// Stats & Buffs
public:
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetMovementSpeed() const { return MovementSpeed; }

	const TArray<FBuffEffect>& GetCurrentlyActiveBuffs() const override;
	void AddBuff(const FBuffEffect& NewBuff) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Buffs")
	void OnBuffAdded(const FBuffEffect& NewBuff);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float										MaxHealth = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float										CurrentHealth = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float										HealPerSecond = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float										MovementSpeed = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	bool										bIsSlowed = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buffs")
	TArray<FBuffEffect>							Buffs;

	//~==============================================================================
	// Lifecycle and events
public:
	FORCEINLINE FEnemyKilledEvent& GetEnemyKilledEvent() { return OnEnemyKilled; }
	FORCEINLINE FEnemyDamageEvent& GetEnemyTookDamageEvent() { return OnEnemyTookDamage; }
	FORCEINLINE FEnemyDamageEvent& GetEnemyBlockedDamageEvent() { return OnEnemyBlockedDamage; }
	FORCEINLINE bool CanBeAttacked() const { return bIsDying == false; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	float TakePhysicalDamage(UStatEffect const * const StatEffectCDO);
	float TakeMagicalDamage(UStatEffect const * const StatEffectCDO);
	float TakeSlowDamage(UStatEffect const * const StatEffectCDO);
	float TakeStunDamage(UStatEffect const * const StatEffectCDO);

	bool CanBlockThisAttack() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Stats")
	void OnHealthChanged();

private:
	bool										bIsDying = false;
	bool										bHasDied = false;
	float										DeathEffectTime = 0.25f;
	FEnemyKilledEvent							OnEnemyKilled;
	FEnemyDamageEvent							OnEnemyTookDamage;
	FEnemyDamageEvent							OnEnemyBlockedDamage;
	
};
