// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ClearPlayerReference.h"

// engine includes
#include "BehaviorTree/BlackboardComponent.h"

// game includes
#include "AI/Shared/EAE_6900EnemyController.h"

UBTTask_ClearPlayerReference::UBTTask_ClearPlayerReference(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Clear Player Reference";
}

EBTNodeResult::Type UBTTask_ClearPlayerReference::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEAE_6900EnemyController* EnemyController = Cast<AEAE_6900EnemyController>(OwnerComp.GetAIOwner());
	if (EnemyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	EnemyController->GetBlackboardComponent()->SetValueAsObject(EnemyController->GetPlayerKeyName(), nullptr);
	return EBTNodeResult::Succeeded;
}
