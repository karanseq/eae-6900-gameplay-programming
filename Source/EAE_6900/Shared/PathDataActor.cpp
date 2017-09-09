// Fill out your copyright notice in the Description page of Project Settings.

#include "PathDataActor.h"

// engine includes
#include "Components/SplineComponent.h"

// Sets default values
APathDataActor::APathDataActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APathDataActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathDataActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector APathDataActor::GetLocation(float Time) const
{
	return SplineComponent->GetWorldLocationAtTime(Time);
}

FRotator APathDataActor::GetDirection(float Time) const
{
	return SplineComponent->GetWorldRotationAtTime(Time);	
}

float APathDataActor::GetDuration() const
{
	return SplineComponent->Duration;
}


