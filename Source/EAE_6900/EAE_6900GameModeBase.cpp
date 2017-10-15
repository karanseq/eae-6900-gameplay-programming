// Fill out your copyright notice in the Description page of Project Settings.

#include "EAE_6900GameModeBase.h"

// game includes
#include "EAE_6900PlayerController.h"
#include "EAE_6900Pawn.h"

AEAE_6900GameModeBase::AEAE_6900GameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = AEAE_6900PlayerController::StaticClass();
	HUDClass = nullptr;
	GameSessionClass = nullptr;
	SpectatorClass = nullptr;
	ReplaySpectatorPlayerControllerClass = nullptr;
}





