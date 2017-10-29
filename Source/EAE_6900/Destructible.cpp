// Fill out your copyright notice in the Description page of Project Settings.

#include "Destructible.h"

// engine includes
#include "Components/BoxComponent.h"

// game includes

ADestructible::ADestructible(const FObjectInitializer& ObjectInitializer)
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

float ADestructible::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	// reduce health
	++HitsTaken;

	// call blueprint event
	UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	ReceiveAnyDamage(DamageAmount, DamageTypeCDO, EventInstigator, DamageCauser);

	if (HitsTaken >= HitsToDestroy)
	{
		Destroy();
	}

	return DamageAmount;
}

void ADestructible::BeginPlay()
{
	Super::BeginPlay();
	
}

