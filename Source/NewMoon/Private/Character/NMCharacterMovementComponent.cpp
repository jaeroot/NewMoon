// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NMCharacterMovementComponent.h"

#include "Character/NMCharacter.h"
#include "Net/UnrealNetwork.h"


UNMCharacterMovementComponent::UNMCharacterMovementComponent()
{
	StandWalkSpeed = 600.0f;
	CrouchWalkSpeed = 300.0f;
	ProneWalkSpeed = 150.0f;
}

void UNMCharacterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UNMCharacterMovementComponent, CurrentCharacterMotion);
	DOREPLIFETIME(UNMCharacterMovementComponent, bIsCrouched);
	DOREPLIFETIME(UNMCharacterMovementComponent, bIsProne);
	DOREPLIFETIME(UNMCharacterMovementComponent, CrouchPlaying);
	DOREPLIFETIME(UNMCharacterMovementComponent, PronePlaying);
}

void UNMCharacterMovementComponent::UpdateBasedMovement(float DeltaSeconds)
{
	// Super::UpdateBasedMovement(DeltaSeconds);
	return;
}

void UNMCharacterMovementComponent::SetWalkSpeed()
{
	switch (CurrentCharacterMotion)
	{
	case ECharacterMotion::ECM_Stand:
		MaxWalkSpeed = StandWalkSpeed;
		break;
	case ECharacterMotion::ECM_Crouch:
		MaxWalkSpeed = CrouchWalkSpeed;
		break;
	case ECharacterMotion::ECM_Prone:
		MaxWalkSpeed = ProneWalkSpeed;
		break;
	default:
		break;
	}
}

void UNMCharacterMovementComponent::SetCharacterMotion(ECharacterMotion NewCharacterMotion)
{
	NMCHECK(CurrentCharacterMotion != NewCharacterMotion);

	ANMCharacter* Character = Cast<ANMCharacter>(GetCharacterOwner());
	if (!Character)
		return;
	
	bool Crouched = false;
	if (CurrentCharacterMotion == ECharacterMotion::ECM_Crouch)
		Crouched = true;
	
	CurrentCharacterMotion = NewCharacterMotion;
	
	switch (CurrentCharacterMotion)
	{
	case ECharacterMotion::ECM_Stand:
		if (Crouched)
		{
			bIsCrouched = false;
			CrouchPlaying = true;
			Character->CrouchTimeline->ReverseFromEnd();
		}
		else
		{
			bIsProne = false;
			PronePlaying = true;
			Character->ProneTimeline->ReverseFromEnd();
		}
		break;
	case ECharacterMotion::ECM_Crouch:
		bIsCrouched = true;
		SetWalkSpeed();
		CrouchPlaying = true;
		Character->CrouchTimeline->PlayFromStart();
		break;
	case ECharacterMotion::ECM_Prone:
		bIsProne = true;
		SetWalkSpeed();
		PronePlaying = true;
		Character->ProneTimeline->PlayFromStart();
		break;
	default:
		break;
	}
}

void UNMCharacterMovementComponent::OnRep_CurrentCharacterMotion(ECharacterMotion LastCharacterMotion)
{	
	switch (CurrentCharacterMotion)
	{
	case ECharacterMotion::ECM_Stand:
		break;
	case ECharacterMotion::ECM_Crouch:
		SetWalkSpeed();
		break;
	case ECharacterMotion::ECM_Prone:
		SetWalkSpeed();
		break;
	default:
		break;
	}
}

void UNMCharacterMovementComponent::DoCrouch()
{
	if (bIsProne)
		return;

	if (CrouchPlaying || PronePlaying)
		return;
	
	ANMCharacter* Character = Cast<ANMCharacter>(GetCharacterOwner());
	if (!Character)
		return;

	if (bIsCrouched)
	{
		if (!Character->CheckCanStand(Character->CrouchHalfHeight))
		{
			SetCharacterMotion(ECharacterMotion::ECM_Stand);
		}
	}
	else if (!IsFalling())
	{
		SetCharacterMotion(ECharacterMotion::ECM_Crouch);
	}
}

void UNMCharacterMovementComponent::DoProne()
{
	if (bIsCrouched)
		return;

	if (CrouchPlaying || PronePlaying)
		return;

	ANMCharacter* Character = Cast<ANMCharacter>(GetCharacterOwner());
	if (!Character)
		return;

	if (bIsProne)
	{		
		if (!Character->CheckCanStand(Character->ProneHalfHeight))
		{
			SetCharacterMotion(ECharacterMotion::ECM_Stand);
		}
	}
	else if (!IsFalling())
	{
		SetCharacterMotion(ECharacterMotion::ECM_Prone);
	}
}