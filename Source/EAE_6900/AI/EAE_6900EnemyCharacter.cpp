// Fill out your copyright notice in the Description page of Project Settings.

#include "EAE_6900EnemyCharacter.h"

// engine includes
#include "Perception/PawnSensingComponent.h"

// game includes
#include "AI/EAE_6900EnemyController.h"

// Sets default values
AEAE_6900EnemyCharacter::AEAE_6900EnemyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SetPeripheralVisionAngle(75.0f);
	PawnSensingComponent->SightRadius = 1500.0f;
	PawnSensingComponent->HearingThreshold = 500.0f;
	PawnSensingComponent->LOSHearingThreshold = 1000.0f;
}

//~==============================================================================
// Behavior

void AEAE_6900EnemyCharacter::OnSeePlayer(APawn* Pawn)
{
	if (AEAE_6900EnemyController* EnemyController = Cast<AEAE_6900EnemyController>(GetController()))
	{
		EnemyController->TargetSighted(Pawn);
	}
}

void AEAE_6900EnemyCharacter::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	if (AEAE_6900EnemyController* EnemyController = Cast<AEAE_6900EnemyController>(GetController()))
	{
		if (PawnInstigator != this)
		{
			GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Orange, FString::Printf(TEXT("Heard sound with volume %f at %s"), Volume, *Location.ToString()));
			EnemyController->TargetSighted(PawnInstigator);
		}
	}
}

//~==============================================================================
// Game Loop

// Called when the game starts or when spawned
void AEAE_6900EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEAE_6900EnemyCharacter::OnSeePlayer);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &AEAE_6900EnemyCharacter::OnHearNoise);
}

// Called every frame
void AEAE_6900EnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
