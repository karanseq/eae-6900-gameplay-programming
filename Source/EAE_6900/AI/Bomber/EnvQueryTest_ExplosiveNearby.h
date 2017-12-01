// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"

// game includes

#include "EnvQueryTest_ExplosiveNearby.generated.h"

/**
 * 
 */
UCLASS()
class EAE_6900_API UEnvQueryTest_ExplosiveNearby : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = Explosive)
	float ScanRadius = 100.0f;
	
protected:
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;	

};
