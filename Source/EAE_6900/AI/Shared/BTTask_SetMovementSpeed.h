// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

// game includes

#include "BTTask_SetMovementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class EAE_6900_API UBTTask_SetMovementSpeed : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI)
	float MovementSpeedToSet = 0.0f;

};
