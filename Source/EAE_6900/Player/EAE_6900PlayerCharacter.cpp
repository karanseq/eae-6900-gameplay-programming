// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "EAE_6900PlayerCharacter.h"

// engine includes
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Engine/CollisionProfile.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/Material.h"

// game includes
#include "Shared/Explosive.h"

AEAE_6900PlayerCharacter::AEAE_6900PlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/Player/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Create the noise emitter component
	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	HealthBar->SetupAttachment(RootComponent);
}

//~==============================================================================
// Behavior

void AEAE_6900PlayerCharacter::MakeFootstepNoise()
{
	MakeNoise(1.0f, this, GetActorLocation());
}

void AEAE_6900PlayerCharacter::MakeAttackNoise()
{
	MakeNoise(1.0f, this, GetActorLocation());
}

void AEAE_6900PlayerCharacter::PlantExplosive()
{
	static constexpr float PlantExplosiveOffset = 100.0f;
	FTransform SpawnTransform(GetActorLocation() + GetActorForwardVector() * PlantExplosiveOffset);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AExplosive* Bomb = GetWorld()->SpawnActor<AExplosive>(ExplosiveClass, SpawnTransform, SpawnParams);
	Bomb->SetTargetType(EExplosiveTargetType::Enemy);

	OnExplosivePlanted(Bomb);
}

float AEAE_6900PlayerCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float PreviousHealth = Health;
	Health -= Health > Damage ? Damage : Damage - Health;
	ReceiveAnyDamage(PreviousHealth - Health, nullptr, nullptr, nullptr);
	if (Health <= 0)
	{
		Die();
	}
	return PreviousHealth - Health;
}

//~==============================================================================
// Game Loop

void AEAE_6900PlayerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}

	if (HealthBar != nullptr)
	{
		const FRotator LookAtCamera = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TopDownCameraComponent->GetComponentLocation());
		HealthBar->SetWorldRotation(LookAtCamera);
	}
}
