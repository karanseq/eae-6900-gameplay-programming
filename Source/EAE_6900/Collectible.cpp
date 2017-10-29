// Fill out your copyright notice in the Description page of Project Settings.

#include "Collectible.h"

// engine includes
#include "Components/BoxComponent.h"

// game includes
#include "EAE_6900Pawn.h"

ACollectible::ACollectible(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->SetCollisionProfileName(FName(TEXT("OverlapAllDynamic")));
	BoxComponent->CanCharacterStepUpOn = ECB_No;
	BoxComponent->bDynamicObstacle = true;
	RootComponent = BoxComponent;

}

//~==============================================================================
// Behavior

void ACollectible::BeginPlay()
{
	Super::BeginPlay();
	
}

