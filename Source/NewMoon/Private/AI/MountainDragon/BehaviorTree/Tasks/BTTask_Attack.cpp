// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MountainDragon/BehaviorTree/Tasks/BTTask_Attack.h"

#include "AIController.h"
#include "AI/MountainDragon/NMMountainDragon.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	// att = 0;
	// bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto* Character = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetCharacter());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	int rand = FMath::RandRange(1, 4);
	Character->MulticastAttack(rand);
	
	FAttackTaskMemory* MyMemory = CastInstanceNodeMemory<FAttackTaskMemory>(NodeMemory);
	MyMemory->att++;
	
	NMLOG(Warning, TEXT("att: %d"), MyMemory->att);

	// att++;
	// NMLOG(Warning, TEXT("Att: %d"), att);

	return EBTNodeResult::Succeeded;
}

uint16 UBTTask_Attack::GetInstanceMemorySize() const
{
	return sizeof(FAttackTaskMemory);
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
