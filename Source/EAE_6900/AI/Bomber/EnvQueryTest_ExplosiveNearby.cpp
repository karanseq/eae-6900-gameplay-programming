// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvQueryTest_ExplosiveNearby.h"

// engine includes
#include "WorldCollision.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

// game includes
#include "Shared/Explosive.h"

UEnvQueryTest_ExplosiveNearby::UEnvQueryTest_ExplosiveNearby(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Cost = EEnvTestCost::High;
	ValidItemType = UEnvQueryItemType_Point::StaticClass();
	TestPurpose = EEnvTestPurpose::FilterAndScore;
	FilterType = EEnvTestFilterType::Match;
}

void UEnvQueryTest_ExplosiveNearby::RunTest(FEnvQueryInstance& QueryInstance) const
{
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		FHitResult HitResult;
		const FQuat TestQuat = FQuat::Identity;
		const FVector TestLocation = GetItemLocation(QueryInstance, It);
		FCollisionShape SphereShape = FCollisionShape::MakeSphere(ScanRadius);

		bool bBlockingHit = GetWorld()->SweepSingleByChannel(HitResult, TestLocation, TestLocation, TestQuat, ECC_WorldStatic, SphereShape);
		It.SetScore(TestPurpose, FilterType, bBlockingHit, false);
	}
}

FText UEnvQueryTest_ExplosiveNearby::GetDescriptionTitle() const
{
	return FText::FromString(FString::Printf(TEXT("Explosives Nearby")));
}

FText UEnvQueryTest_ExplosiveNearby::GetDescriptionDetails() const
{
	return DescribeBoolTestParams(FString::Printf(TEXT("Explosives Nearby")));
}
