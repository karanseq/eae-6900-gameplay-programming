// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// game includes
#include "SaveGameData.h"

#include "Collectible.generated.h"

// forward declarations
class UBoxComponent;

// type declarations
UENUM(BlueprintType)
enum class ECollectibleType : uint8
{
	None,
	Ammo,
	Health
};

UCLASS()
class EAE_6900_API ACollectible : public AActor,
	public ISaveable
{
	GENERATED_UCLASS_BODY()

	//~==============================================================================
	// Behavior

public:
	FORCEINLINE ECollectibleType GetType() const { return Type; }
	FORCEINLINE float GetDelta() const { return Delta; }

	void Collected(bool bWithAnimation = true);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Collectible)
	FName GUID = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Collectible)
	ECollectibleType Type = ECollectibleType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collectible)
	float Delta = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = Collectible)
	bool bCollected = false;

	//~==============================================================================
	// Save Game
public:
	void SubmitDataToBeSaved(FLevelSaveData& LevelSaveData) const override;

	//~==============================================================================
	// Components
public:
	FORCEINLINE UBoxComponent* GetBoxComponent() const { return BoxComponent; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collectible, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent = nullptr;

};
