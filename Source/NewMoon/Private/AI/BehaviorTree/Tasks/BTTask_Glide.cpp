// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviorTree/Tasks/BTTask_Glide.h"

#include "AIController.h"
#include "AI/MountainDragon/NMMountainDragon.h"

UBTTask_Glide::UBTTask_Glide()
{
	NodeName = TEXT("StartGlideOld");
}

EBTNodeResult::Type UBTTask_Glide::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto* Character = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetCharacter());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Character->StartGlide();
	
	return EBTNodeResult::Succeeded;
}
