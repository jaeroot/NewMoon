// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NMAnimInstance.h"

#include "Character/NMCharacter.h"
#include "KismetAnimationLibrary.h"

void UNMAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	NMCharacter = Cast<ANMCharacter>(TryGetPawnOwner());
}

void UNMAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (NMCharacter == nullptr)
	{
		NMCharacter = Cast<ANMCharacter>(TryGetPawnOwner());
	}
	if (!::IsValid(NMCharacter)) return;


	FVector Velocity = NMCharacter->GetVelocity();
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, NMCharacter->GetActorRotation());
	Velocity.Z = 0.0f;
	Speed = Velocity.Size();


	bIsInAir = NMCharacter->GetMovementComponent()->IsFalling();
	
}
