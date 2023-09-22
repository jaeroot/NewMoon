// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviorTree/Tasks/BTTask_GlideStart.h"

#include "AIController.h"
#include "AI/MountainDragon/NMMountainDragon.h"

UBTTask_GlideStart::UBTTask_GlideStart(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Start Glide");
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_GlideStart::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* NMMountainDragon = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetPawn());
	if (NMMountainDragon == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	FBTGlideStartTaskMemory* MyMemory = CastInstanceNodeMemory<FBTGlideStartTaskMemory>(NodeMemory);
	MyMemory->bIsFinished = false;

	NMMountainDragon->SetGlide(true);
	NMMountainDragon->ServerTakeOff();
	NMMountainDragon->TakeOffEnd.AddLambda([this, MyMemory]() -> void
	{
		MyMemory->bIsFinished = true;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_GlideStart::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const FBTGlideStartTaskMemory* MyMemory = CastInstanceNodeMemory<FBTGlideStartTaskMemory>(NodeMemory);
	if (MyMemory->bIsFinished)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

uint16 UBTTask_GlideStart::GetInstanceMemorySize() const
{
	return sizeof(FBTGlideStartTaskMemory);
}