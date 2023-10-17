// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviorTree/Tasks/BTTask_GlideAttack.h"

#include "AIController.h"
#include "AI/MountainDragon/NMMountainDragon.h"
#include "AI/MountainDragon/NMMountainDragonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GlideAttack::UBTTask_GlideAttack(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("GlideAttack");
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_GlideAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* NMMountainDragon = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetPawn());
	if (NMMountainDragon == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FBTGlideAttackTaskMemory* MyMemory = CastInstanceNodeMemory<FBTGlideAttackTaskMemory>(NodeMemory);
	MyMemory->bIsFinished = false;
	
	NMMountainDragon->ServerGlideAttack();
	NMMountainDragon->GlideAttackEnd.AddLambda([this, MyMemory]() -> void
	{
		MyMemory->bIsFinished = true;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_GlideAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const FBTGlideAttackTaskMemory* MyMemory = CastInstanceNodeMemory<FBTGlideAttackTaskMemory>(NodeMemory);
	if (MyMemory->bIsFinished)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_GlideAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANMMountainDragonAIController::GlideAttackedKey, true);
}

uint16 UBTTask_GlideAttack::GetInstanceMemorySize() const
{
	return sizeof(FBTGlideAttackTaskMemory);
}
