// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "AIController.h"
#include "NMMountainDragonAIController.generated.h"

/**
 * 
 */
UCLASS()
class NEWMOON_API ANMMountainDragonAIController : public AAIController
{
	GENERATED_BODY()

public:
	ANMMountainDragonAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	static const FName IsBattleStateKey;
	static const FName HPPercentageKey;
	static const FName CanGlideAttackKey;
	static const FName CanFireBallAttackKey;
	static const FName CanFireSpreadAttackKey;
	static const FName TargetKey;
	static const FName BaseLocationKey;
	static const FName FireLocationKey;
	static const FName FireTargetKey;
	static const FName GlideTargetKey;
	
private:
	UPROPERTY()
	class UBlackboardData* BBAsset;
	
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	
};
