// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetMovementSpeed.h"

// engine includes
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// game includes
#include "AI/Shared/EAE_6900EnemyCharacter.h"

UBTTask_SetMovementSpeed::UBTTask_SetMovementSpeed(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Set Movement Speed";
}

EBTNodeResult::Type UBTTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEAE_6900EnemyCharacter* EnemyCharacter = Cast<AEAE_6900EnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = MovementSpeedToSet;
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
