// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_FindPlayer.generated.h"

/**
 * 
 */
UCLASS()
class EAE_6900_API UEnvQueryContext_FindPlayer : public UEnvQueryContext
{
	GENERATED_UCLASS_BODY()
	
	void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
	
};
