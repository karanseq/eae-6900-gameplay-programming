// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// game includes

#include "Destructible.generated.h"

// forward declarations
class UBoxComponent;

UCLASS()
class EAE_6900_API ADestructible : public AActor
{
	GENERATED_UCLASS_BODY()

	//~==============================================================================
	// Behavior

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Destructible)
	FName GUID = NAME_None;

	UPROPERTY(BlueprintReadOnly, Category = Destructible)
	int32 HitsTaken = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Destructible)
	int32 HitsToDestroy = 5;

	//~==============================================================================
	// Components
public:
	FORCEINLINE UBoxComponent* GetBoxComponent() const { return BoxComponent; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Destructible, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent = nullptr;

};
