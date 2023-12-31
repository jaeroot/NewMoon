// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "Animation/AnimInstance.h"
#include "NMMountainDragonAnimInstance.generated.h"

class ANMMountainDragon;

DECLARE_MULTICAST_DELEGATE(FAttackDelegate);

UCLASS()
class NEWMOON_API UNMMountainDragonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	FAttackDelegate OnAttack;

private:
	UFUNCTION()
	void AnimNotify_Attack();
	
private:
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	ANMMountainDragon* NMMountainDragon;
	
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	FVector Velocity;
	
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float Speed;
	
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float Direction;
	
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bIsBattleState;
	
	UPROPERTY(BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	bool bGlideAttack;
	
	UPROPERTY(BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	bool bFly;
	
	UPROPERTY(BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	bool bFireBallAttack;
	
	UPROPERTY(BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	bool bFireSpreadAttack;

	UPROPERTY(BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

	bool CheckFireSpread;
	bool CheckFireBall;
	

	
};
