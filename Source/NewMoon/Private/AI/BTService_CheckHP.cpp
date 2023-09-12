// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckHP.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "AI/NMMountainDragon.h"
#include "AI/NMMountainDragonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"

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

			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ANMMountainDragonAIController::BaseLocationKey, FVector(-15000.0f, -3000.0f, 4000.0f));
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ANMMountainDragonAIController::FireLocationKey, FVector(-15000.0f, 0.0f, 6000.0f));

			for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *It->GetName());
				if (*It->GetName() == FString("TargetPoint_1"))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANMMountainDragonAIController::FireTargetKey, *It);
				}
				
				if (*It->GetName() == FString("TargetPoint_0"))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANMMountainDragonAIController::GlideTargetKey, *It);
				}
			}
		}
	}

	if (!FireSpreadAttacked)
	{
		if (HPPercentage < 80)
		{
			FireSpreadAttacked = true;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANMMountainDragonAIController::CanFireSpreadAttackKey, true);
		}
	}

	if (!GlideAttacked)
	{
		if (HPPercentage < 50)
		{
			GlideAttacked = true;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANMMountainDragonAIController::CanGlideAttackKey, true);
		}
	}

	if (!FireBallAttacked)
	{
		if (HPPercentage < 30)
		{
			FireBallAttacked = true;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANMMountainDragonAIController::CanFireBallAttackKey, true);
		}
	}
}
