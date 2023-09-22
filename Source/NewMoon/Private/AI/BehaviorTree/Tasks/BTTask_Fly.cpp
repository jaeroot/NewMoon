// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviorTree/Tasks/BTTask_Fly.h"

#include "AIController.h"
#include "AI/MountainDragon/NMMountainDragon.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Fly::UBTTask_Fly()
{
	NodeName = TEXT("StartFlyOld");
}

EBTNodeResult::Type UBTTask_Fly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto* Character = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetCharacter());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// Character->StartFly();

	return EBTNodeResult::Succeeded;
}