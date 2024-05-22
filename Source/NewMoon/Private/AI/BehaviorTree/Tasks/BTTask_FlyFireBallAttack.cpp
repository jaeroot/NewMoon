// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviorTree/Tasks/BTTask_FlyFireBallAttack.h"

#include "AI/MountainDragon/NMMountainDragon.h"
#include "AI/MountainDragon/NMMountainDragonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FlyFireBallAttack::UBTTask_FlyFireBallAttack(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("FireBallAttack");
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_FlyFireBallAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* NMMountainDragon = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetPawn());
	if (NMMountainDragon == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	NMMountainDragon->SetIsInProgress(true);
	NMMountainDragon->ServerFireBallAttack();

	return EBTNodeResult::InProgress;
}

void UBTTask_FlyFireBallAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto* NMMountainDragon = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetPawn());
	if (NMMountainDragon == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (!NMMountainDragon->GetIsInProgress())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_FlyFireBallAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANMMountainDragonAIController::FireBallAttackedKey, true);
}

uint16 UBTTask_FlyFireBallAttack::GetInstanceMemorySize() const
{
	return sizeof(UBTTask_FlyFireBallAttack);
}


