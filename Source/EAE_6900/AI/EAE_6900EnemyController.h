// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// game includes

#include "EAE_6900EnemyController.generated.h"

// forward declarations
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
	void TargetSighted(APawn* InTarget);

	FORCEINLINE const TArray<AActor*>& GetWaypoints() const { return Waypoints; }
	FORCEINLINE const FName& GetCurrentWaypointKeyName() const { return CurrentWaypointKeyName; }
	
	FORCEINLINE AActor* GetCurrentWaypoint() const { return BlackboardComponent ? Cast<AActor>(BlackboardComponent->GetValueAsObject(CurrentWaypointKeyName)) : nullptr; }
	FORCEINLINE APawn* GetTarget() const { return BlackboardComponent ? Cast<APawn>(BlackboardComponent->GetValueAsObject(TargetKeyName)) : nullptr; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = AI)
	TArray<AActor*> Waypoints;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName CurrentWaypointKeyName = "CurrentWaypoint";

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName TargetKeyName = "Target";

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
