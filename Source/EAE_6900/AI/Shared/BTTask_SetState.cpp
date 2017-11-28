// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetState.h"

// engine includes
#include "BehaviorTree/BlackboardComponent.h"

// game includes
#include "AI/Shared/EAE_6900EnemyController.h"

UBTTask_SetState::UBTTask_SetState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

EBTNodeResult::Type UBTTask_SetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEAE_6900EnemyController* EnemyController = Cast<AEAE_6900EnemyController>(OwnerComp.GetAIOwner());
	if (EnemyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	EnemyController->GetBlackboardComponent()->SetValueAsEnum(EnemyController->GetStateKeyName(), static_cast<uint8>(StateToSet));
	return EBTNodeResult::Succeeded;
}
