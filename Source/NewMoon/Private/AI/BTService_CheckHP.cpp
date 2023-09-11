// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckHP.h"

#include "AIController.h"
#include "AI/NMMountainDragon.h"
#include "AI/NMMountainDragonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckHP::UBTService_CheckHP()
{
	NodeName = TEXT("CheckHP");
	Interval = 1.0f;
	BattleStarted = false;
	GlideAttacked = false;
	FireBallAttacked = false;
	FireSpreadAttacked = false;
}

void UBTService_CheckHP::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto* Character = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetCharacter());
	if (Character == nullptr)
	{
		return;
	}

	int HPPercentage = Character->GetHPPercentage();
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(ANMMountainDragonAIController::HPPercentageKey, HPPercentage);
	
	if (!BattleStarted)
	{
		if (HPPercentage < 100)
		{
			BattleStarted = true;
			Character->bIsBattleState = true;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANMMountainDragonAIController::IsBattleStateKey, true);
		}
	}

	if (!FireSpreadAttacked)
	{
		if (HPPercentage < 80)
		{
			FireSpreadAttacked = true;
			Character->bFireSpreadAttack = true;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANMMountainDragonAIController::CanFireSpreadAttackKey, true);
		}
	}

	if (!GlideAttacked)
	{
		if (HPPercentage < 50)
		{
			GlideAttacked = true;
			Character->bGlideAttack = true;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANMMountainDragonAIController::CanGlideAttackKey, true);
		}
	}

	if (!FireBallAttacked)
	{
		if (HPPercentage < 30)
		{
			FireBallAttacked = true;
			Character->bFireBallAttack = true;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANMMountainDragonAIController::CanFireBallAttackKey, true);
		}
	}
}
