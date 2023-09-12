// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Fly.h"

#include "AIController.h"
#include "AI/NMMountainDragon.h"
#include "AI/NMMountainDragonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_Fly::UBTTask_Fly()
{
	NodeName = TEXT("StartFly");
}

EBTNodeResult::Type UBTTask_Fly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto* Character = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetCharacter());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Character->StartFly();

	return EBTNodeResult::Succeeded;
}