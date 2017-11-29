// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_AttackPlayer.h"

// engine includes
#include "AIController.h"

// game includes
#include "AI/Shared/EAE_6900EnemyCharacter.h"

UBTTaskNode_AttackPlayer::UBTTaskNode_AttackPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Attack Player";
}

EBTNodeResult::Type UBTTaskNode_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEAE_6900EnemyCharacter* EnemyCharacter = Cast<AEAE_6900EnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		EnemyCharacter->Attack();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
