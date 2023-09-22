// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MountainDragon/NMMountainDragonAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

const FName ANMMountainDragonAIController::IsBattleStateKey(TEXT("IsBattleState"));
const FName ANMMountainDragonAIController::HPPercentageKey(TEXT("HPPercentage"));
const FName ANMMountainDragonAIController::GlideAttackedKey(TEXT("GlideAttacked"));
const FName ANMMountainDragonAIController::FireBallAttackedKey(TEXT("FireBallAttacked"));
const FName ANMMountainDragonAIController::FireSpreadAttackedKey(TEXT("FireSpreadAttacked"));
const FName ANMMountainDragonAIController::TargetKey(TEXT("Target"));
const FName ANMMountainDragonAIController::BaseLocationKey(TEXT("BaseLocation"));
const FName ANMMountainDragonAIController::FireLocationKey(TEXT("FireLocation"));
const FName ANMMountainDragonAIController::FireTargetKey(TEXT("FireTarget"));
const FName ANMMountainDragonAIController::GlideTargetKey(TEXT("GlideTarget"));

ANMMountainDragonAIController::ANMMountainDragonAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprints/AI/MountainDragon/BB_NMMountainDragon.BB_NMMountainDragon"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
		
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/AI/MountainDragon/BT_NMMountainDragon.BT_NMMountainDragon"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ANMMountainDragonAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (UseBlackboard(BBAsset, BlackboardComponent))
	{
		if (!RunBehaviorTree(BTAsset))
		{
			NMLOG(Error, TEXT("AIController couldn't run behavior tree"));
		}
	}
}

void ANMMountainDragonAIController::OnUnPossess()
{
	Super::OnUnPossess();

	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}
