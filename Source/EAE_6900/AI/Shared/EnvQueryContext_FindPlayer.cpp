// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvQueryContext_FindPlayer.h"

// engine includes
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

// game includes
#include "AI/EAE_6900EnemyController.h"

UEnvQueryContext_FindPlayer::UEnvQueryContext_FindPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void UEnvQueryContext_FindPlayer::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	if (AActor* OwningActor = Cast<AActor>(QueryInstance.Owner.Get()))
	{
		if (AEAE_6900EnemyController* EnemyController = Cast<AEAE_6900EnemyController>(OwningActor->GetInstigatorController()))
		{
			if (EnemyController->GetTarget())
			{
				UEnvQueryItemType_Actor::SetContextHelper(ContextData, EnemyController->GetTarget());
			}
		}
	}
}
