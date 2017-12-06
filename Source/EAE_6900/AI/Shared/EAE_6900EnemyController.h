// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// game includes
#include "EAE_6900AITypes.h"

#include "EAE_6900EnemyController.generated.h"

// forward declarations
class AExplosive;
class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class EAE_6900_API AEAE_6900EnemyController : public AAIController
{
	GENERATED_UCLASS_BODY()

	//~==============================================================================
	// Behavior
public:
	virtual void Possess(APawn* InPawn) override;
	void PlayerSighted(APawn* InTarget);
	void OnBombPlanted(AExplosive* Bomb);

	FORCEINLINE const TArray<AActor*>& GetWaypoints() const { return Waypoints; }
	FORCEINLINE const TArray<AActor*>& GetBombsPlanted() const { return GBombsPlanted; }

	// BB Keys
	FORCEINLINE const FName& GetCurrentWaypointKeyName() const { return CurrentWaypointKeyName; }
	FORCEINLINE const FName& GetPlayerKeyName() const { return PlayerKeyName; }
	FORCEINLINE const FName& GetStateKeyName() const { return StateKeyName; }
	FORCEINLINE const FName& GetAttackCooldownKeyName() const { return AttackCooldownKeyName; }
	FORCEINLINE const FName& GetExplosivePlantLocationKeyName() const { return ExplosivePlantLocationKeyName; }

	// BB Values
	FORCEINLINE AActor* GetCurrentWaypoint() const { return BlackboardComponent ? Cast<AActor>(BlackboardComponent->GetValueAsObject(CurrentWaypointKeyName)) : nullptr; }
	FORCEINLINE APawn* GetPlayer() const { return BlackboardComponent ? Cast<APawn>(BlackboardComponent->GetValueAsObject(PlayerKeyName)) : nullptr; }
	FORCEINLINE EAIState GetState() const { return BlackboardComponent ? static_cast<EAIState>(BlackboardComponent->GetValueAsEnum(StateKeyName)) : EAIState::Invalid; }
	FORCEINLINE float GetAttackCooldown() const { return BlackboardComponent ? BlackboardComponent->GetValueAsFloat(AttackCooldownKeyName) : 0.0f; }
	FORCEINLINE FVector GetExplosivePlantLocation() const { return BlackboardComponent ? BlackboardComponent->GetValueAsVector(ExplosivePlantLocationKeyName) : FVector::ZeroVector; }

protected:
	UFUNCTION()
	void OnBombExploded(AActor* Bomb);

protected:
	UPROPERTY(BlueprintReadOnly, Category = AI)
	TArray<AActor*> Waypoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AI)
	FName CurrentWaypointKeyName = "CurrentWaypoint";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AI)
	FName PlayerKeyName = "Player";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AI)
	FName StateKeyName = "State";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AI)
	FName AttackCooldownKeyName = "AttackCooldown";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AI)
	FName ExplosivePlantLocationKeyName = "ExplosivePlantLocation";

private:
	static TArray<AActor*> GBombsPlanted;

	//~==============================================================================
	// Components
public:
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorTreeComponent() const { return BehaviorTreeComponent; }
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	UBehaviorTreeComponent* BehaviorTreeComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent = nullptr;
	
};
