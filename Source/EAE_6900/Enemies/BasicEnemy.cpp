// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicEnemy.h"

// engine includes
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "TimerManager.h"

// game includes
#include "Shared/PathDataActor.h"

// Sets default values
ABasicEnemy::ABasicEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = Box;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();
	HealingTicker = 1.0f;
}

// Called every frame
void ABasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDying)
	{
		if (!bHasDied)
		{
			DeathEffectTime -= DeltaTime;
			if (DeathEffectTime > 0.0f)
			{
				FVector Scale = GetActorScale3D();
				SetActorScale3D(Scale + Scale * 0.01f);

				FRotator Rotation = GetActorRotation();
				Rotation.Pitch += 1;
				SetActorRotation(FQuat(Rotation));
			}
			else
			{
				Destroy();
				bHasDied = true;
			}
		}

		return;
	}

	HealingTicker -= DeltaTime;
	if (HealingTicker < 0.0f)
	{
		HealingTicker = 1.0f;
		CurrentHealth = FMath::Clamp(CurrentHealth + HealPerSecond, 0.0f, MaxHealth);
	}

	if (PathDataActor)
	{
		AccumulatedTime += (DeltaTime * MovementSpeed);

		FHitResult HitResult;
		SetActorLocationAndRotation(PathDataActor->GetLocation(AccumulatedTime),
			PathDataActor->GetDirection(AccumulatedTime),
			false,
			&HitResult,
			ETeleportType::None
		);

		if (AccumulatedTime > PathDataActor->GetDuration())
		{
			PathDataActor = nullptr;
			Destroy();
		}
	}
}

float ABasicEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (bHasDied || bIsDying)
	{
		return 0.0f;
	}

	UStatEffect const * const StatEffectCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UStatEffect>() : GetDefault<UStatEffect>();

	if (CanBlockThisAttack())
	{
		OnEnemyBlockedDamage.ExecuteIfBound(GetName(), 0.0f, StatEffectCDO->Stat);
		return 0.0f;
	}

	float TotalDamage = 0.0f;
	switch (StatEffectCDO->Stat)
	{
	case EStatKind::Physical:
		TotalDamage += TakePhysicalDamage(StatEffectCDO);
		break;

	case EStatKind::Magic:
		TotalDamage += TakeMagicalDamage(StatEffectCDO);
		break;

	case EStatKind::Slow:
		TakeSlowDamage(StatEffectCDO);
		break;

	case EStatKind::Stun:
		TakeStunDamage(StatEffectCDO);
		break;
	}

	if (CurrentHealth <= 0.0f)
	{
		bIsDying = true;
		OnEnemyKilled.ExecuteIfBound(GetName(), StatEffectCDO->Stat);
		Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}

	return TotalDamage;
}

float ABasicEnemy::TakePhysicalDamage(UStatEffect const * const StatEffectCDO)
{
	float CurrentArmor = 0.0f;
	for (const auto& BuffEffect : Buffs)
	{
		if (BuffEffect.Buff == EBuffKind::Armor)
		{
			CurrentArmor += FMath::FloorToFloat(FMath::RandRange(BuffEffect.Min, BuffEffect.Max));
		}
	}

	const float DamageTaken = FMath::Clamp(FMath::FloorToFloat(FMath::RandRange(StatEffectCDO->Min, StatEffectCDO->Max)) - CurrentArmor, 0.0f, MaxHealth);
	CurrentHealth -= DamageTaken;

	OnHealthChanged();
	OnEnemyTookDamage.ExecuteIfBound(GetName(), DamageTaken, StatEffectCDO->Stat);

	return DamageTaken;
}

float ABasicEnemy::TakeMagicalDamage(UStatEffect const * const StatEffectCDO)
{
	float CurrentMagicResist = 0.0f;
	for (const auto& BuffEffect : Buffs)
	{
		if (BuffEffect.Buff == EBuffKind::MagicResist)
		{
			CurrentMagicResist += FMath::FloorToFloat(FMath::RandRange(BuffEffect.Min, BuffEffect.Max));
		}
	}

	const float DamageTaken = FMath::Clamp(FMath::FloorToFloat(FMath::RandRange(StatEffectCDO->Min, StatEffectCDO->Max)) - CurrentMagicResist, 0.0f, MaxHealth);
	CurrentHealth -= DamageTaken;

	OnHealthChanged();
	OnEnemyTookDamage.ExecuteIfBound(GetName(), DamageTaken, StatEffectCDO->Stat);

	return DamageTaken;
}

float ABasicEnemy::TakeSlowDamage(UStatEffect const * const StatEffectCDO)
{
	if (bIsSlowed)
	{
		return 0.0f;
	}
	bIsSlowed = true;

	const float AmountSlowed = FMath::RandRange(StatEffectCDO->Min, StatEffectCDO->Max);
	MovementSpeed -= AmountSlowed;
	OnEnemyTookDamage.ExecuteIfBound(GetName(), AmountSlowed, StatEffectCDO->Stat);

	FTimerDelegate TimerDelegate;
	FTimerHandle TimerHandle;
	TimerDelegate.BindLambda([AmountSlowed, this]() {
		MovementSpeed += AmountSlowed;
		bIsSlowed = false;
	});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, StatEffectCDO->Duration, /*InbLoop = */false);

	return AmountSlowed;
}

float ABasicEnemy::TakeStunDamage(UStatEffect const * const StatEffectCDO)
{
	return 0.0f;
}

bool ABasicEnemy::CanBlockThisAttack() const
{
	bool bCanBlockThisAttack = false;
	for (const auto& BuffEffect : Buffs)
	{
		if (BuffEffect.Buff == EBuffKind::Block)
		{
			const float Chance = FMath::FRandRange(0.0f, 1.0f);
			bCanBlockThisAttack |= (Chance < BuffEffect.Max);
		}
	}
	return bCanBlockThisAttack;
}

const TArray<FBuffEffect>& ABasicEnemy::GetCurrentlyActiveBuffs() const
{
	return Buffs;
}

void ABasicEnemy::AddBuff(const FBuffEffect& NewBuff)
{
	OnBuffAdded(NewBuff);
}

