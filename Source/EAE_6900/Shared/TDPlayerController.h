// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EAE_6900_API ATDPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

	//~==============================================================================
	// Behavior
public:
	FORCEINLINE int32 GetHealth() const { return Health; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Lifecycle")
	void OnGameWin();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Lifecycle")
	void OnGameLose();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Lifecycle")
	void OnLostHealth();

	void DecrementHealth();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Game Lifecycle")
	int32							Health = 10;

	UPROPERTY(BlueprintReadOnly, Category = "Game Lifecycle")
	bool							bIsDead = false;
};
