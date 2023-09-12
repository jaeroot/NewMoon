// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_EndFly.h"

#include "AIController.h"
#include "AI/NMMountainDragon.h"

UBTTask_EndFly::UBTTask_EndFly()
{
	NodeName = TEXT("EndFly");
}

EBTNodeResult::Type UBTTask_EndFly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto* Character = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetCharacter());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Character->EndFly();

	return EBTNodeResult::Succeeded;
}
