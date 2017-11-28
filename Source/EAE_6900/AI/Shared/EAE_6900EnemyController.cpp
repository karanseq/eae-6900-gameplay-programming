// Fill out your copyright notice in the Description page of Project Settings.

#include "EAE_6900EnemyController.h"

// engine includes
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

// game includes
#include "EAE_6900EnemyCharacter.h"

AEAE_6900EnemyController::AEAE_6900EnemyController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
}

//~==============================================================================
// Behavior

void AEAE_6900EnemyController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	if (AEAE_6900EnemyCharacter* MyPawn = Cast<AEAE_6900EnemyCharacter>(InPawn))
	{
		// initialize the blackboard
		if (MyPawn->GetBehaviorTree()->BlackboardAsset)
		{
			BlackboardComponent->InitializeBlackboard(*MyPawn->GetBehaviorTree()->BlackboardAsset);
		}

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), Waypoints);

		BehaviorTreeComponent->StartTree(*MyPawn->GetBehaviorTree());
	}
}

void AEAE_6900EnemyController::TargetSighted(APawn* InTarget)
{
	BlackboardComponent->SetValueAsObject(TargetKeyName, InTarget);
}
