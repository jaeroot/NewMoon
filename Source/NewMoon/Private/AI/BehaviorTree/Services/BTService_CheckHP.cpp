// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviorTree/Services/BTService_CheckHP.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "AI/MountainDragon/NMMountainDragon.h"
#include "AI/MountainDragon/NMMountainDragonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"

UBTService_CheckHP::UBTService_CheckHP()
{
	NodeName = TEXT("CheckHP");
	Interval = 1.0f;
	BattleStarted = false;
	bCreateNodeInstance = true;
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
}
