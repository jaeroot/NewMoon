// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FlyEnd.generated.h"

struct FBTFlyEndTaskMemory
{
	// check Land() Finished
	bool bIsFinished;
};

UCLASS()
class NEWMOON_API UBTTask_FlyEnd : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FlyEnd(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual uint16 GetInstanceMemorySize() const override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};
