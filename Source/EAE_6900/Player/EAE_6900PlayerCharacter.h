// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// game includes

#include "EAE_6900PlayerCharacter.generated.h"

// forward declarations
class UPawnNoiseEmitterComponent;

UCLASS(Blueprintable)
class AEAE_6900PlayerCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	//~==============================================================================
	// Behavior
public:
	UFUNCTION(BlueprintCallable, Category = AI)
	void MakeFootstepNoise();

	//~==============================================================================
	// Game Loop
public:
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	//~==============================================================================
	// Components
public:
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }
	FORCEINLINE class UPawnNoiseEmitterComponent* GetNoiseEmitter() const { return NoiseEmitter; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	class UPawnNoiseEmitterComponent* NoiseEmitter;
};

