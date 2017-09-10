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
	Physical					UMETA(DisplayName = "Physical Damage"),
	Magic						UMETA(DisplayName = "Magic Damage"),
	Stun						UMETA(DisplayName = "Stun"),
	Slow						UMETA(DisplayName = "Slow Movement"),
};

UCLASS(Abstract)
class EAE_6900_API UStatEffect : public UDamageType
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	EStatKind Stat = EStatKind::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Min = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Max = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Duration = 0.0f;

};
