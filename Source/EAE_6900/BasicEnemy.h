// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasicEnemy.generated.h"

// forward declarations
class UStaticMeshComponent;
class APathDataActor;

// delegate declarations
DECLARE_DELEGATE_OneParam(FEnemySpawnedEvent, const FString&);
DECLARE_DELEGATE_OneParam(FEnemyKilledEvent, const FString&);
DECLARE_DELEGATE_TwoParams(FEnemyProgressEvent, const FString&, int32);

UCLASS()
class EAE_6900_API ABasicEnemy : public APawn
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
	UStaticMeshComponent*						Mesh;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetPathDataActor(APathDataActor* PathDataActorIn) { PathDataActor = PathDataActorIn; }
	FORCEINLINE FEnemySpawnedEvent& GetEnemySpawnedEvent() { return OnEnemySpawned; }
	FORCEINLINE FEnemyKilledEvent& GetEnemyKilledEvent() { return OnEnemyKilled; }
	FORCEINLINE FEnemyProgressEvent& GetEnemyProgressEvent() { return OnEnemyMadeProgress; }

private:
	APathDataActor*								PathDataActor = nullptr;
	float										AccumulatedTime = 0.0f;

	FEnemySpawnedEvent							OnEnemySpawned;
	FEnemyKilledEvent							OnEnemyKilled;
	FEnemyProgressEvent							OnEnemyMadeProgress;

};
