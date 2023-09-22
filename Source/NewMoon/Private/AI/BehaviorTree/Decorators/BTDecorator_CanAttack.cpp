// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviorTree/Decorators/BTDecorator_CanAttack.h"

#include "AIController.h"
#include "AI/MountainDragon/NMMountainDragon.h"
#include "AI/MountainDragon/NMMountainDragonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/NMCharacter.h"

UBTDecorator_CanAttack::UBTDecorator_CanAttack()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	auto* Character = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetCharacter());
	if (Character == nullptr)
	{
		return false;
	}
	
	auto Target = Cast<ANMCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANMMountainDragonAIController::TargetKey));
	if (Target == nullptr)
	{
		return false;
	}
	
	bool bResult = (Target->GetDistanceTo(Character) <= 650.0f);
	return bResult;
}
