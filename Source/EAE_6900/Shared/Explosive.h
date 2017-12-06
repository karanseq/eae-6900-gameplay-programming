// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// game includes

#include "Explosive.generated.h"

// forward declarations
class USphereComponent;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EExplosiveTargetType : uint8
{
	None,
	Player,
	Enemy,
};

UCLASS()
class EAE_6900_API AExplosive : public AActor
{
	GENERATED_UCLASS_BODY()

	//~==============================================================================
	// Behavior

public:
	FORCEINLINE void SetTargetType(EExplosiveTargetType TargetTypeIn) { TargetType = TargetTypeIn; }
	FORCEINLINE EExplosiveTargetType GetTargetType() const { return TargetType; }
	FORCEINLINE float GetBaseDamage() const { return BaseDamage; }
	FORCEINLINE float GetMinDamage() const { return MinDamage; }
	FORCEINLINE float GetRadius() const { return Radius; }

protected:
	void OnConstruction(const FTransform& Transform) override;
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintImplementableEvent, Category = Explosive)
	void DoExplosionEffects();

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Explosive)
	EExplosiveTargetType TargetType = EExplosiveTargetType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Explosive)
	float BaseDamage = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Explosive)
	float MinDamage = 50.0f;

	UPROPERTY(BlueprintReadOnly, Category = Explosive)
	float Radius = 0.0f;

	//~==============================================================================
	// Components

public:
	FORCEINLINE USphereComponent* GetSphereCollision() const { return SphereCollision; }
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh = nullptr;
};
