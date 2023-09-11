// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Turn.h"

#include "AIController.h"
#include "AI/NMMountainDragon.h"
#include "AI/NMMountainDragonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/NMCharacter.h"

UBTTask_Turn::UBTTask_Turn()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_Turn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto* Character = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetCharacter());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<ANMCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANMMountainDragonAIController::TargetKey));
	if (Target == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector LookVector = Target->GetActorLocation() - Character->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Character->SetActorRotation(FMath::RInterpTo(Character->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}
