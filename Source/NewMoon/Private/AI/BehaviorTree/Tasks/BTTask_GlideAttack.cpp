// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviorTree/Tasks/BTTask_GlideAttack.h"

#include "AIController.h"
#include "AI/MountainDragon/NMMountainDragon.h"

UBTTask_GlideAttack::UBTTask_GlideAttack()
{
	NodeName = TEXT("GlideAttack");
}

EBTNodeResult::Type UBTTask_GlideAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto* Character = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetCharacter());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Character->GlideAttack();
	
	return EBTNodeResult::Succeeded;
}
