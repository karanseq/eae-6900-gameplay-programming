// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE(FGameOverSignature)

UCLASS()
class EAE_6900_API ATDPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

	//~==============================================================================
	// Behavior
public:
	FORCEINLINE int32 GetHealth() const { return Health; }
	FORCEINLINE bool GetIsDead() const { return bIsDead; }
	FORCEINLINE bool GetIsGameOver() const { return bIsGameOver; }
	FORCEINLINE FGameOverSignature& GetOnGameOver() { return OnGameOver; }

	void NotifyAllWavesCleared();
	void DecrementHealth();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Game Lifecycle")
	void OnGameWin();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Lifecycle")
	void OnGameLose();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Lifecycle")
	void OnLostHealth();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Game Lifecycle")
	int32							Health = 10;

	UPROPERTY(BlueprintReadOnly, Category = "Game Lifecycle")
	bool							bIsDead = false;

	UPROPERTY(BlueprintReadOnly, Category = "Game Lifecycle")
	bool							bIsGameOver = false;

private:
	FGameOverSignature				OnGameOver;

};
