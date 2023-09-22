// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FlyStart.generated.h"

struct FBTFlyStartTaskMemory
{
	// check TakeOff() Finished
	bool bIsFinished;
};

UCLASS()
class NEWMOON_API UBTTask_FlyStart : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FlyStart(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual uint16 GetInstanceMemorySize() const override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
