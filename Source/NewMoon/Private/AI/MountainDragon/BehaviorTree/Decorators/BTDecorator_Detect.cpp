// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MountainDragon/BehaviorTree/Decorators/BTDecorator_Detect.h"

#include "AIController.h"
#include "AI/MountainDragon/NMMountainDragon.h"
#include "AI/MountainDragon/NMMountainDragonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/NMCharacter.h"

UBTDecorator_Detect::UBTDecorator_Detect()
{
	NodeName = TEXT("Detect");
}

bool UBTDecorator_Detect::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	auto* Character = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetCharacter());
	if (Character == nullptr)
	{
		return false;
	}
	
	UWorld* World = Character->GetWorld();
	FVector Center = Character->GetActorLocation();
	FVector DetectBox = FVector(1500.0f, 1500.0f, 500.0f);
	
	if (!World)
	{
		return false;
	}
	
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, Character);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECC_Camera,
		FCollisionShape::MakeBox(DetectBox),
		CollisionQueryParam
	);
	
	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			auto NMCharacter = Cast<ANMCharacter>(OverlapResult.GetActor());
			if (NMCharacter)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANMMountainDragonAIController::TargetKey, OverlapResult.GetActor());
				DrawDebugBox(World, Center, DetectBox, FColor::Green, false, 0.2f);
	
				return true;
			}
		}
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANMMountainDragonAIController::TargetKey, nullptr);
	DrawDebugBox(World, Center, DetectBox, FColor::Red, false, 0.2f);
	
	return false;
}
