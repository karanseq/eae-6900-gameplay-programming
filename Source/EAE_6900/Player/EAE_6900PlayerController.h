// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EAE_6900PlayerController.generated.h"

// forward declarations
class AEAE_6900PlayerCharacter;

UCLASS()
class AEAE_6900PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEAE_6900PlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;
	uint32 bPlayerDead : 1;

	AEAE_6900PlayerCharacter* PlayerCharacter = nullptr;

	// Begin PlayerController interface
	virtual void Possess(APawn* aPawn) override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
};


