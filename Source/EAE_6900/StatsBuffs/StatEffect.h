// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"

#include "StatEffect.generated.h"

UENUM(BlueprintType)
enum class EStatKind : uint8
{
	None						UMETA(DisplayName = "Invalid Type"),
	Physical					UMETA(DisplayName = "Physical"),
	Magic						UMETA(DisplayName = "Magic"),
	Stun						UMETA(DisplayName = "Stun"),
	Slow						UMETA(DisplayName = "Slow"),
};

UCLASS(Abstract)
class EAE_6900_API UStatEffect : public UDamageType
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat Effect")
	EStatKind Stat = EStatKind::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat Effect")
	float Delta = 0.0f;

};
