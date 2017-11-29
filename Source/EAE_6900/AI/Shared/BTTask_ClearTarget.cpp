// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ClearTarget.h"

// engine includes
#include "BehaviorTree/BlackboardComponent.h"

// game includes
#include "AI/Shared/EAE_6900EnemyController.h"

UBTTask_ClearTarget::UBTTask_ClearTarget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

EBTNodeResult::Type UBTTask_ClearTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEAE_6900EnemyController* EnemyController = Cast<AEAE_6900EnemyController>(OwnerComp.GetAIOwner());
	if (EnemyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	EnemyController->GetBlackboardComponent()->SetValueAsObject(EnemyController->GetPlayerKeyName(), nullptr);
	return EBTNodeResult::Succeeded;
}
