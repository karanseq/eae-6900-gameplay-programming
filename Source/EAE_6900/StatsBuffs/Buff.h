// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Buff.generated.h"

UENUM(BlueprintType)
enum class EBuffKind : uint8
{
	None							UMETA(DisplayName = "Invalid Type"),
	Armor							UMETA(DisplayName = "Armor Buff"),
	MagicResist						UMETA(DisplayName = "Magic Resistance BUff"),
	Block							UMETA(DisplayName = "Blocking Buff"),
	MoveSpeed						UMETA(DisplayName = "Movement Speed Buff"),
	AttackSpeed						UMETA(DisplayName = "Attack Speed Buff"),
	Health							UMETA(DisplayName = "Healing Buff"),
};

USTRUCT(BlueprintType)
struct EAE_6900_API FBuffEffect
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buffs")
	EBuffKind Buff = EBuffKind::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buffs")
	float Min = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buffs")
	float Max = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buffs")
	float Duration = 0.0f;

};