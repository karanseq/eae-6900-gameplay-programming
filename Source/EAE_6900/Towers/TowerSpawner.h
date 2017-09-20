// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerSpawner.generated.h"

// forward declarations
class ABasicTower;
class UStaticMeshComponent;

UCLASS()
class EAE_6900_API ATowerSpawner : public AActor
{
	GENERATED_UCLASS_BODY()

	//~==============================================================================
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent*						Mesh;

	//~==============================================================================
	// Behavior
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<ABasicTower> PhysicalTowerBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<ABasicTower> MagicalTowerBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<ABasicTower> SlowingTowerBP;

protected:	
	void NotifyActorOnClicked(FKey ButtonPressed = EKeys::LeftMouseButton) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void ShowTowerSelection();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnPhysicalTowerSelected();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnMagicalTowerSelected();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnSlowingTowerSelected();

private:
	void SpawnTower(const TSubclassOf<ABasicTower>& TowerClass);

	//~==============================================================================
	// Unit Test helpers
#if	WITH_EDITOR
public:
	FORCEINLINE const TSubclassOf<ABasicTower>& GetPhysicalTowerBP() const { return PhysicalTowerBP; }
	FORCEINLINE const TSubclassOf<ABasicTower>& GetMagicalTowerBP() const { return MagicalTowerBP; }
	FORCEINLINE const TSubclassOf<ABasicTower>& GetSlowingTowerBP() const { return SlowingTowerBP; }

	void UnitTest_SpawnTower(const TSubclassOf<ABasicTower>& TowerClass)
	{
		SpawnTower(TowerClass);
	}
#endif

};
