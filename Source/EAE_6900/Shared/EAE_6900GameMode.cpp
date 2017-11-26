// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

// engine includes
#include "EAE_6900GameMode.h"
#include "UObject/ConstructorHelpers.h"

// game includes
#include "Player/EAE_6900PlayerController.h"
#include "Player/EAE_6900PlayerCharacter.h"

AEAE_6900GameMode::AEAE_6900GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AEAE_6900PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/Player/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}