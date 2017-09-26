// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EAE_6900Hud.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class EAE_6900_API AEAE_6900Hud : public AHUD
{
    GENERATED_BODY()

public:
    AEAE_6900Hud();

    /** Font used to render the vehicle info */
    UPROPERTY()
    UFont* HUDFont;

    // Begin AHUD interface
    virtual void DrawHUD() override;
    // End AHUD interface
};
