// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_StartFly.h"

#include "AIController.h"
#include "AI/NMMountainDragon.h"

UBTTask_StartFly::UBTTask_StartFly(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("StartFly");
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_StartFly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	auto* NMMountainDragon = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetPawn());
	if (NMMountainDragon == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	FBTStartFlyTaskMemory* MyMemory = CastInstanceNodeMemory<FBTStartFlyTaskMemory>(NodeMemory);
	MyMemory->bIsFinished = false;

	NMMountainDragon->ServerTakeOff();
	NMMountainDragon->TakeOffEnd.AddLambda([this, MyMemory]() -> void
	{
		MyMemory->bIsFinished = true;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_StartFly::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const FBTStartFlyTaskMemory* MyMemory = CastInstanceNodeMemory<FBTStartFlyTaskMemory>(NodeMemory);
	if (MyMemory->bIsFinished)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

uint16 UBTTask_StartFly::GetInstanceMemorySize() const
{
	return sizeof(FBTStartFlyTaskMemory);
}
