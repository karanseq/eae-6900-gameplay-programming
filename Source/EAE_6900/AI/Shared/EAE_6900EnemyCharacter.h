// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// game includes

#include "EAE_6900EnemyCharacter.generated.h"

// forward declarations
class UBehaviorTree;
class UPawnSensingComponent;

UCLASS()
class EAE_6900_API AEAE_6900EnemyCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	//~==============================================================================
	// Behavior
public:
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	UFUNCTION(BlueprintImplementableEvent, Category = AI)
	void Attack();

	UFUNCTION(BlueprintImplementableEvent, Category = AI)
	void Die();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = AI)
	UBehaviorTree* BehaviorTree = nullptr;

	UPROPERTY(VisibleAnywhere, Category = AI)
	UPawnSensingComponent* PawnSensingComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AI)
	float Health = 100.0f;

	//~==============================================================================
	// Game Loop
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
