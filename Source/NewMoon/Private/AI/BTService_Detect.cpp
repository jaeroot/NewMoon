// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"

#include "AIController.h"
#include "AI/NMMountainDragon.h"
#include "AI/NMMountainDragonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/NMCharacter.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 3.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	NMLOG_S(Warning);
	auto* Character = Cast<ANMMountainDragon>(OwnerComp.GetAIOwner()->GetCharacter());
	if (Character == nullptr)
	{
		return;
	}

	UWorld* World = Character->GetWorld();
	FVector Center = Character->GetActorLocation();
	FVector DetectBox = FVector(3000.0f, 3000.0f, 500.0f);

	if (!World)
	{
		return;
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

				return;
			}
		}
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANMMountainDragonAIController::TargetKey, nullptr);
	DrawDebugBox(World, Center, DetectBox, FColor::Red, false, 0.2f);
}
