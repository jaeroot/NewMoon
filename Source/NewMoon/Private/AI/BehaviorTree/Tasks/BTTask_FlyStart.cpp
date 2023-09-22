// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviorTree/Tasks/BTTask_FlyStart.h"

#include "AIController.h"
#include "AI/MountainDragon/NMMountainDragon.h"

UBTTask_FlyStart::UBTTask_FlyStart(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Start Fly");
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_FlyStart::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	auto* NMMountainDragon = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetPawn());
	if (NMMountainDragon == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	FBTFlyStartTaskMemory* MyMemory = CastInstanceNodeMemory<FBTFlyStartTaskMemory>(NodeMemory);
	MyMemory->bIsFinished = false;

	NMMountainDragon->SetFly(true);
	NMMountainDragon->ServerTakeOff();
	NMMountainDragon->TakeOffEnd.AddLambda([this, MyMemory]() -> void
	{
		MyMemory->bIsFinished = true;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_FlyStart::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const FBTFlyStartTaskMemory* MyMemory = CastInstanceNodeMemory<FBTFlyStartTaskMemory>(NodeMemory);
	if (MyMemory->bIsFinished)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

uint16 UBTTask_FlyStart::GetInstanceMemorySize() const
{
	return sizeof(FBTFlyStartTaskMemory);
}
