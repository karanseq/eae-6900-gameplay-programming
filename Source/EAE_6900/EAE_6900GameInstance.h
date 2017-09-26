// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

// game includes
#include "EAE_6900.h"
#include "EAE_6900Bullet.h"

#include "EAE_6900GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EAE_6900_API UEAE_6900GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UEAE_6900GameInstance();

    UPROPERTY(EditAnywhere, Category = AssetReferences)
    TSubclassOf<AEAE_6900Bullet> DefaultBulletBP;
};
