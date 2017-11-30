// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_AttackPlayer.h"

// engine includes
#include "AIController.h"

// game includes
#include "AI/Shared/EAE_6900AITypes.h"
#include "AI/Shared/EAE_6900EnemyCharacter.h"
#include "AI/Shared/EAE_6900EnemyController.h"
#include "Player/EAE_6900PlayerCharacter.h"

UBTTaskNode_AttackPlayer::UBTTaskNode_AttackPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Attack Player";
}

EBTNodeResult::Type UBTTaskNode_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// check if the player is alive
	if (AEAE_6900EnemyController* EnemyController = Cast<AEAE_6900EnemyController>(OwnerComp.GetAIOwner()))
	{
		AEAE_6900PlayerCharacter* PlayerCharacter = Cast<AEAE_6900PlayerCharacter>(EnemyController->GetPlayer());
		if (PlayerCharacter && PlayerCharacter->IsDead())
		{
			EnemyController->GetBlackboardComponent()->SetValueAsEnum(EnemyController->GetStateKeyName(), static_cast<uint8>(EAIState::Patrol));
			return EBTNodeResult::Succeeded;
		}
	}

	// tell the character to attack
	if (AEAE_6900EnemyCharacter* EnemyCharacter = Cast<AEAE_6900EnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		EnemyCharacter->Attack();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
