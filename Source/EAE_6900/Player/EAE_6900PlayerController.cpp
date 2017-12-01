// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "EAE_6900PlayerController.h"

// engine includes
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"

// game includes
#include "Player/EAE_6900PlayerCharacter.h"

AEAE_6900PlayerController::AEAE_6900PlayerController()
{
	bShowMouseCursor = true;
	bPlayerDead = false;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AEAE_6900PlayerController::Possess(APawn* aPawn)
{
	Super::Possess(aPawn);

	PlayerCharacter = Cast<AEAE_6900PlayerCharacter>(aPawn);
}

void AEAE_6900PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// check if the pawn died
	bPlayerDead |= PlayerCharacter ? PlayerCharacter->IsDead() : bPlayerDead;

	// keep updating the destination every tick while desired
	if (!bPlayerDead && bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AEAE_6900PlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AEAE_6900PlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AEAE_6900PlayerController::OnSetDestinationReleased);
}

void AEAE_6900PlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}

void AEAE_6900PlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NavSys && (Distance > 120.0f))
		{
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AEAE_6900PlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AEAE_6900PlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}
