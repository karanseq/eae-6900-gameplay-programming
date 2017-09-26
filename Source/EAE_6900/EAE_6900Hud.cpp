// Fill out your copyright notice in the Description page of Project Settings.

#include "EAE_6900Hud.h"

// engine includes
#include "Engine/Canvas.h"
#include "UObject/ConstructorHelpers.h"

// game includes
#include "EAE_6900Pawn.h"

AEAE_6900Hud::AEAE_6900Hud()
{
    static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
    HUDFont = Font.Object;
}

void AEAE_6900Hud::DrawHUD()
{
    Super::DrawHUD();

    // calculate ratio from 720p
    const float HUDXRatio = Canvas->SizeX / 1280.0f;
    const float HUDYRatio = Canvas->SizeY / 720.0f;

    AEAE_6900Pawn* Vehicle = Cast<AEAE_6900Pawn>(GetOwningPawn());
    if (Vehicle)
    {
        FVector2D ScaleVec(HUDXRatio * 1.4f, HUDYRatio * 1.4f);

        // speed
        FCanvasTextItem SpeedTextItem(FVector2D(HUDXRatio * 100.0f, HUDYRatio * 550.0f), Vehicle->SpeedText, HUDFont, FLinearColor::White);
        SpeedTextItem.Scale = ScaleVec;
        Canvas->DrawItem(SpeedTextItem);

        // rpm
        FCanvasTextItem RPMTextItem(FVector2D(HUDXRatio * 100.0f, HUDYRatio * 585.0f), Vehicle->RPMText, HUDFont, FLinearColor::White);
        RPMTextItem.Scale = ScaleVec;
        Canvas->DrawItem(RPMTextItem);

        //// health
        //FCanvasTextItem HealthTextItem(FVector2D(HUDXRatio * 100.0f, HUDYRatio * 620.0f), Vehicle->HealthText, HUDFont, FLinearColor::White);
        //HealthTextItem.Scale = ScaleVec;
        //Canvas->DrawItem(HealthTextItem);

        //// ammo
        //FCanvasTextItem AmmoTextItem(FVector2D(HUDXRatio * 100.0f, HUDYRatio * 655.0f), Vehicle->AmmoText, HUDFont, FLinearColor::White);
        //AmmoTextItem.Scale = ScaleVec;
        //Canvas->DrawItem(AmmoTextItem);
    }
}

