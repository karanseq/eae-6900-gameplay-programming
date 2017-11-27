// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

// game includes

#include "BTTaskNode_ChooseWaypoint.generated.h"

/**
 * 
 */
UCLASS()
class EAE_6900_API UBTTaskNode_ChooseWaypoint : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;	
	
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}

};
