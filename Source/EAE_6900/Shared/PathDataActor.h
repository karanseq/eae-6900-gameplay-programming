// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathDataActor.generated.h"

// forward declarations
class USplineComponent;

UCLASS()
class EAE_6900_API APathDataActor : public AActor
{
	GENERATED_UCLASS_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USplineComponent*							SplineComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetLocation(float Time) const;
	FRotator GetDirection(float Time) const;
	float GetDuration() const;
	
};
