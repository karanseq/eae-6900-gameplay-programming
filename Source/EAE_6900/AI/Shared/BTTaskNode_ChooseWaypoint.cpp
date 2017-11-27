// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_ChooseWaypoint.h"

// engine includes
#include "BehaviorTree/BlackboardComponent.h"

// game includes
#include "AI/EAE_6900EnemyController.h"

UBTTaskNode_ChooseWaypoint::UBTTaskNode_ChooseWaypoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

EBTNodeResult::Type UBTTaskNode_ChooseWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEAE_6900EnemyController* EnemyController = Cast<AEAE_6900EnemyController>(OwnerComp.GetAIOwner());
	if (EnemyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AActor* CurrentWaypoint = EnemyController->GetCurrentWaypoint();
	const TArray<AActor*> Waypoints = EnemyController->GetWaypoints();

	if (Waypoints.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	AActor* NextWaypoint = CurrentWaypoint;
	do
	{
		NextWaypoint = Waypoints[FMath::RandRange(0, Waypoints.Num() - 1)];
	} while (CurrentWaypoint == NextWaypoint);


	EnemyController->GetBlackboardComponent()->SetValueAsObject(EnemyController->GetCurrentWaypointKeyName(), NextWaypoint);
	return EBTNodeResult::Succeeded;
}


