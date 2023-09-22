// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviorTree/Tasks/BTTask_FlyFireSpreadAttack.h"

#include "AI/MountainDragon/NMMountainDragon.h"
#include "AI/MountainDragon/NMMountainDragonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FlyFireSpreadAttack::UBTTask_FlyFireSpreadAttack(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("FireSpreadAttack");
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_FlyFireSpreadAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* NMMountainDragon = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetPawn());
	if (NMMountainDragon == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// NMMountainDragon->FireSpreadAttack();

	return EBTNodeResult::Succeeded;
}

void UBTTask_FlyFireSpreadAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	
}

void UBTTask_FlyFireSpreadAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANMMountainDragonAIController::FireSpreadAttackedKey, true);
}

uint16 UBTTask_FlyFireSpreadAttack::GetInstanceMemorySize() const
{
	return Super::GetInstanceMemorySize();
}
