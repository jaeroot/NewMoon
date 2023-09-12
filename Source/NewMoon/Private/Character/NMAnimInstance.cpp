// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NMAnimInstance.h"

#include "Character/NMCharacter.h"
#include "KismetAnimationLibrary.h"
#include "Character/NMCharacterMovementComponent.h"

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


	Velocity = NMCharacter->GetVelocity();
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, NMCharacter->GetActorRotation());
	Velocity.Z = 0.0f;
	Speed = Velocity.Size();
	bWeaponEquipped = NMCharacter->IsWeaponEquipped();


	if (UNMCharacterMovementComponent* CharacterMovementComponent = Cast<UNMCharacterMovementComponent>(NMCharacter->GetCharacterMovement()))
	{
		bIsInAir = CharacterMovementComponent->IsFalling();
		bIsCrouched = CharacterMovementComponent->bIsCrouched;
		bIsProne = CharacterMovementComponent->bIsProne;
		CurrentCharacterMotion = CharacterMovementComponent->GetCurrentCharacterMotion();
	}
	
}
