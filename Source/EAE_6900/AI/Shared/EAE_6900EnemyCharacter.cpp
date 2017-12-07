// Fill out your copyright notice in the Description page of Project Settings.

#include "EAE_6900EnemyCharacter.h"

// engine includes
#include "BehaviorTree/BehaviorTree.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/PawnSensingComponent.h"

// game includes
#include "AI/Shared/EAE_6900EnemyController.h"
#include "Player/EAE_6900PlayerCharacter.h"
#include "Shared/Explosive.h"

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

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetCollisionProfileName(TEXT("NoCollision"));
	HealthBar->SetupAttachment(RootComponent);
}

//~==============================================================================
// Behavior

void AEAE_6900EnemyCharacter::OnSeePlayer(APawn* Pawn)
{
	if (AEAE_6900EnemyController* EnemyController = Cast<AEAE_6900EnemyController>(GetController()))
	{
		EnemyController->PlayerSighted(Pawn);
	}
}

void AEAE_6900EnemyCharacter::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	if (AEAE_6900EnemyController* EnemyController = Cast<AEAE_6900EnemyController>(GetController()))
	{
		if (PawnInstigator != this)
		{
			EnemyController->PlayerSighted(PawnInstigator);
		}
	}
}

float AEAE_6900EnemyCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float PreviousHealth = Health;

	if (bImmuneToExplosives &&
		DamageCauser->IsA(AExplosive::StaticClass()))
	{
		return 0.0f;
	}

	Health -= Health > Damage ? Damage : Damage - Health;
	ReceiveAnyDamage(PreviousHealth - Health, nullptr, nullptr, nullptr);

	if (Health <= 0)
	{
		if (AEAE_6900EnemyController* EnemyController = Cast<AEAE_6900EnemyController>(GetController()))
		{
			EnemyController->GetBehaviorTreeComponent()->StopTree();
		}

		Die();
		OnEnemyDied.Broadcast(this);
	}
	return PreviousHealth - Health;
}

//~==============================================================================
// Game Loop

void AEAE_6900EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEAE_6900EnemyCharacter::OnSeePlayer);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &AEAE_6900EnemyCharacter::OnHearNoise);
}

void AEAE_6900EnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HealthBar != nullptr)
	{
		if (AEAE_6900PlayerCharacter* PlayerCharacter = Cast<AEAE_6900PlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
		{
			const FRotator LookAtCamera = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerCharacter->GetTopDownCameraComponent()->GetComponentLocation());
			HealthBar->SetWorldRotation(LookAtCamera);
		}
	}
}
