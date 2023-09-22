// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviorTree/Tasks/BTTask_FlyEnd.h"

#include "AIController.h"
#include "AI/MountainDragon/NMMountainDragon.h"

UBTTask_FlyEnd::UBTTask_FlyEnd(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("End Fly");
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_FlyEnd::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* NMMountainDragon = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetPawn());
	if (NMMountainDragon == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FBTFlyEndTaskMemory* MyMemory = CastInstanceNodeMemory<FBTFlyEndTaskMemory>(NodeMemory);
	MyMemory->bIsFinished = false;

	NMMountainDragon->ServerLand();
	NMMountainDragon->LandEnd.AddLambda([this, MyMemory]() -> void
	{
		MyMemory->bIsFinished = true;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_FlyEnd::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const FBTFlyEndTaskMemory* MyMemory = CastInstanceNodeMemory<FBTFlyEndTaskMemory>(NodeMemory);
	if (MyMemory->bIsFinished)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_FlyEnd::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	auto* NMMountainDragon = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetPawn());
	if (NMMountainDragon == nullptr)
	{
		return;
	}
	NMMountainDragon->SetFly(false);
}

uint16 UBTTask_FlyEnd::GetInstanceMemorySize() const
{
	return sizeof(FBTFlyEndTaskMemory);;
}
