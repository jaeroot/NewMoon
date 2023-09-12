// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_GlideEnd.h"

#include "AIController.h"
#include "AI/NMMountainDragon.h"

UBTTask_GlideEnd::UBTTask_GlideEnd()
{
	NodeName = TEXT("EndGlide");
}

EBTNodeResult::Type UBTTask_GlideEnd::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto* Character = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetCharacter());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Character->EndGlide();
	
	return EBTNodeResult::Succeeded;
}
