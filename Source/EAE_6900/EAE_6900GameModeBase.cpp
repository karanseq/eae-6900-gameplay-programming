// Fill out your copyright notice in the Description page of Project Settings.

#include "EAE_6900GameModeBase.h"

// game includes
#include "EAE_6900Hud.h"
#include "EAE_6900Pawn.h"

AEAE_6900GameModeBase::AEAE_6900GameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = AEAE_6900Pawn::StaticClass();
	HUDClass = AEAE_6900Hud::StaticClass();
}





