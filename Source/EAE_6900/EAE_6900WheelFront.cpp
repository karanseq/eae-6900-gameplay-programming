// Fill out your copyright notice in the Description page of Project Settings.

#include "EAE_6900WheelFront.h"

// engine includes
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"

UEAE_6900WheelFront::UEAE_6900WheelFront()
{
    ShapeRadius = 18.0f;
    ShapeWidth = 15.0f;
    bAffectedByHandbrake = false;
    SteerAngle = 40.f;

    // Setup suspension forces
    SuspensionForceOffset = -4.0f;
    SuspensionMaxRaise = 8.0f;
    SuspensionMaxDrop = 12.0f;
    SuspensionNaturalFrequency = 9.0f;
    SuspensionDampingRatio = 1.05f;

    // Find the tire object and set the data for it
    static ConstructorHelpers::FObjectFinder<UTireConfig> TireData(TEXT("/Game/VehicleAdv/Vehicle/WheelData/Vehicle_FrontTireConfig.Vehicle_FrontTireConfig"));
    TireConfig = TireData.Object;
}