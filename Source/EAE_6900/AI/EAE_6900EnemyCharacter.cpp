// Fill out your copyright notice in the Description page of Project Settings.

#include "EAE_6900EnemyCharacter.h"

// engine includes

// game includes

// Sets default values
AEAE_6900EnemyCharacter::AEAE_6900EnemyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

//~==============================================================================
// Behavior

//~==============================================================================
// Game Loop

// Called when the game starts or when spawned
void AEAE_6900EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEAE_6900EnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
