// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviorTree/Tasks/BTTask_GlideEnd.h"

#include "AIController.h"
#include "AI/MountainDragon/NMMountainDragon.h"

UBTTask_GlideEnd::UBTTask_GlideEnd(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("End Glide");
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_GlideEnd::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* NMMountainDragon = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetPawn());
	if (NMMountainDragon == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FBTGlideEndTaskMemory* MyMemory = CastInstanceNodeMemory<FBTGlideEndTaskMemory>(NodeMemory);
	MyMemory->bIsFinished = false;

	NMMountainDragon->ServerLand();
	NMMountainDragon->LandEnd.AddLambda([this, MyMemory]() -> void
	{
		MyMemory->bIsFinished = true;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_GlideEnd::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const FBTGlideEndTaskMemory* MyMemory = CastInstanceNodeMemory<FBTGlideEndTaskMemory>(NodeMemory);
	if (MyMemory->bIsFinished)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_GlideEnd::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	auto* NMMountainDragon = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetPawn());
	if (NMMountainDragon == nullptr)
	{
		return;
	}
	NMMountainDragon->SetGlide(false);
}

uint16 UBTTask_GlideEnd::GetInstanceMemorySize() const
{
	return sizeof(FBTGlideEndTaskMemory);
}
