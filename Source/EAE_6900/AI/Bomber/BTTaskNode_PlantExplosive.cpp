// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_PlantExplosive.h"

// engine includes

// game includes
#include "AI/Shared/EAE_6900EnemyController.h"
#include "Shared/Explosive.h"

UBTTaskNode_PlantExplosive::UBTTaskNode_PlantExplosive(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Plant Explosive";
}

EBTNodeResult::Type UBTTaskNode_PlantExplosive::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEAE_6900EnemyController* EnemyController = Cast<AEAE_6900EnemyController>(OwnerComp.GetAIOwner()))
	{
		FTransform SpawnTransform(EnemyController->GetExplosivePlantLocation());
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<AExplosive>(ExplosiveClass, SpawnTransform, SpawnParams);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
