// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EAE_6900GameMode.generated.h"

// forward declarations
class AExplosive;

UCLASS(minimalapi)
class AEAE_6900GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AExplosive> ExplosiveBP;

	AEAE_6900GameMode();
};



