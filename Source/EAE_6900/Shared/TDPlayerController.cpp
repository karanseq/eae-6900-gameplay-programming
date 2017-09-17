// Fill out your copyright notice in the Description page of Project Settings.

#include "TDPlayerController.h"


ATDPlayerController::ATDPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void ATDPlayerController::DecrementHealth()
{
	if (bIsDead)
	{
		return;
	}

	--Health;
	if (Health <= 0)
	{
		bIsDead = true;
		OnGameLose();
	}
	else
	{
		OnLostHealth();
	}
}