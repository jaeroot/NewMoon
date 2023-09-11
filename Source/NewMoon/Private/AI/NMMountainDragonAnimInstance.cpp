// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NMMountainDragonAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "AI/NMMountainDragon.h"

void UNMMountainDragonAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	NMMountainDragon = Cast<ANMMountainDragon>(TryGetPawnOwner());
}

void UNMMountainDragonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (NMMountainDragon == nullptr)
	{
		NMMountainDragon = Cast<ANMMountainDragon>(TryGetPawnOwner());
	}
	if (!::IsValid(NMMountainDragon)) return;

	Velocity = NMMountainDragon->GetVelocity();
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, NMMountainDragon->GetActorRotation());
	Velocity.Z = 0.0f;
	Speed = Velocity.Size();
	
	bIsBattleState = NMMountainDragon->bIsBattleState;
	
	bGlideAttack = NMMountainDragon->bGlideAttack;
	bFireBallAttack = NMMountainDragon->bFireBallAttack;
	bFireSpreadAttack = NMMountainDragon->bFireSpreadAttack;
}
