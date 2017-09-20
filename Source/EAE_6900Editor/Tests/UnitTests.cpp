// engine includes
#include "AutomationTest.h"
#include "AutomationCommon.h"
#include "AutomationEditorCommon.h"
#include "UnrealEd.h"

// game includes
#include "Enemies/BasicEnemy.h"
#include "Shared/TDPlayerController.h"
#include "StatsBuffs/StatEffect.h"
#include "Towers/BasicTower.h"
#include "Towers/TowerSpawner.h"

// log declares
DECLARE_LOG_CATEGORY_EXTERN(LogUnitTest, All, All);
DEFINE_LOG_CATEGORY(LogUnitTest);

// constants
#define UNIT_TEST_MAP_NAME "/Game/Levels/UnitTestMap"

//~==============================================================================
// Enemy Take Damage Command and Unit Test

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FBasicEnemyTakeDamage, ABasicEnemy*, InBasicEnemy);

bool FBasicEnemyTakeDamage::Update()
{
	if (InBasicEnemy)
	{
		FDamageEvent DamageEvent;
		DamageEvent.DamageTypeClass = UStatEffect::StaticClass();
		InBasicEnemy->TakeDamage(1.0, DamageEvent, nullptr, nullptr);

		UE_LOG(LogUnitTest, Display, TEXT("Called TakeDamage on %s!"), *InBasicEnemy->GetName());

		return true;
	}
	else
	{
		UE_LOG(LogUnitTest, Display, TEXT("FBasicEnemyTakeDamage command was passed an invalid BasicEnemy!"));
		return false;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEnemyTakeDamageUnitTest, "EAE_6900.UnitTest EnemyTakeDamage", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FEnemyTakeDamageUnitTest::RunTest(const FString& Parameters)
{
	if (GEditor == nullptr)
	{
		return true;
	}

	FAutomationEditorCommonUtils::RunPIE();

	FAutomationEditorCommonUtils::LoadMap(UNIT_TEST_MAP_NAME);

	for (TObjectIterator<ABasicEnemy> It; It; ++It)
	{
		ABasicEnemy* BasicEnemy = *It;
		if (BasicEnemy)
		{
			ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.5f));
			ADD_LATENT_AUTOMATION_COMMAND(FBasicEnemyTakeDamage(BasicEnemy));
			ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.5f));
		}
	}

	return true;
}

//~==============================================================================
// Spawn Tower Command and Unit Test

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FSpawnAllTowers, ATowerSpawner*, InTowerSpawner);

bool FSpawnAllTowers::Update()
{
	if (InTowerSpawner)
	{
		InTowerSpawner->UnitTest_SpawnTower(InTowerSpawner->GetPhysicalTowerBP());
		UE_LOG(LogUnitTest, Display, TEXT("Spawned tower %s!"), *InTowerSpawner->GetPhysicalTowerBP()->GetName());

		InTowerSpawner->UnitTest_SpawnTower(InTowerSpawner->GetMagicalTowerBP());
		UE_LOG(LogUnitTest, Display, TEXT("Spawned tower %s!"), *InTowerSpawner->GetMagicalTowerBP()->GetName());

		InTowerSpawner->UnitTest_SpawnTower(InTowerSpawner->GetSlowingTowerBP());
		UE_LOG(LogUnitTest, Display, TEXT("Spawned tower %s!"), *InTowerSpawner->GetSlowingTowerBP()->GetName());

		return true;
	}
	else
	{
		UE_LOG(LogUnitTest, Display, TEXT("FSpawnAllTowers command was passed an invalid TowerSpawner!"));
		return false;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpawnTowerUnitTest, "EAE_6900.UnitTest SpawnTower", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FSpawnTowerUnitTest::RunTest(const FString& Parameters)
{
	if (GEditor == nullptr)
	{
		return true;
	}

	FAutomationEditorCommonUtils::RunPIE();

	FAutomationEditorCommonUtils::LoadMap(UNIT_TEST_MAP_NAME);

	for (TObjectIterator<ATowerSpawner> It; It; ++It)
	{
		ATowerSpawner* TowerSpawner = *It;
		if (TowerSpawner)
		{
			ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
			ADD_LATENT_AUTOMATION_COMMAND(FSpawnAllTowers(TowerSpawner));
			ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
		}
	}
	return true;
}

//~==============================================================================
// Player Decrement Health Command and Unit Test

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FPlayerDecrementHealth, ATDPlayerController*, InPlayerController);

bool FPlayerDecrementHealth::Update()
{
	if (InPlayerController)
	{
		UE_LOG(LogUnitTest, Display, TEXT("Decrementing player health until death!"));
		for (uint8 I = 0; I < 10; ++I)
		{
			InPlayerController->DecrementHealth();
		}
		return InPlayerController->GetIsDead();
	}
	else
	{
		UE_LOG(LogUnitTest, Display, TEXT("FPlayerDecrementHealth command was passed an invalid PlayerController!"));
		return false;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FReducePlayerHealthUnitTest, "EAE_6900.UnitTest ReducePlayerHealth", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

bool FReducePlayerHealthUnitTest::RunTest(const FString& Parameters)
{
	if (GEditor == nullptr)
	{
		return true;
	}

	FAutomationEditorCommonUtils::RunPIE();

	FAutomationEditorCommonUtils::LoadMap(UNIT_TEST_MAP_NAME);

	for (TObjectIterator<ATDPlayerController> It; It; ++It)
	{
		ATDPlayerController* PlayerController = *It;
		if (PlayerController)
		{
			ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
			ADD_LATENT_AUTOMATION_COMMAND(FPlayerDecrementHealth(PlayerController));
			ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
		}
	}
	return true;
}
