// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckHP.generated.h"

/**
 * 
 */
UCLASS()
class NEWMOON_API UBTService_CheckHP : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckHP();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool BattleStarted;
};
