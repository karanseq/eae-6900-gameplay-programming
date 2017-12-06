// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvQueryContext_FindBombs.h"

// engine includes
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/GameplayStatics.h"

// game includes
#include "Shared/Explosive.h"
#include "Shared/EAE_6900GameMode.h"
#include "AI/Shared/EAE_6900EnemyController.h"

UEnvQueryContext_FindBombs::UEnvQueryContext_FindBombs(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void UEnvQueryContext_FindBombs::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	if (AActor* OwningActor = Cast<AActor>(QueryInstance.Owner.Get()))
	{
		if (AEAE_6900GameMode* GameMode = OwningActor->GetWorld()->GetAuthGameMode<AEAE_6900GameMode>())
		{
			TArray<AActor*> Explosives;
			TSubclassOf<AExplosive> ExplosiveBP = GameMode->ExplosiveBP;
			UGameplayStatics::GetAllActorsOfClass(OwningActor->GetWorld(), ExplosiveBP, Explosives);
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, Explosives);
		}
	}
}
