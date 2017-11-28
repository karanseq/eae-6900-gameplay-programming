// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"

// game includes

#include "EAE_6900AITypes.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Invalid,
	Patrol,
	TargetSighted,
	MovingToHidingSpot,
	MovingToAttack,
	Attacking
};
