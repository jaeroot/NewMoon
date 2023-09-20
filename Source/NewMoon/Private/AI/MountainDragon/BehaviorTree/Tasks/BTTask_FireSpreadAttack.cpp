// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MountainDragon/BehaviorTree/Tasks/BTTask_FireSpreadAttack.h"

#include "AIController.h"
#include "AI/MountainDragon/NMMountainDragon.h"

UBTTask_FireSpreadAttack::UBTTask_FireSpreadAttack(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("FireSpreadAttack");
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_FireSpreadAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* NMMountainDragon = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetPawn());
	if (NMMountainDragon == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	NMMountainDragon->FireSpreadAttack();

	return EBTNodeResult::InProgress;
}

void UBTTask_FireSpreadAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	
}


uint16 UBTTask_FireSpreadAttack::GetInstanceMemorySize() const
{
	return Super::GetInstanceMemorySize();
}