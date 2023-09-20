// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_Detect.generated.h"

/**
 * 
 */
UCLASS()
class NEWMOON_API UBTDecorator_Detect : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_Detect();
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
