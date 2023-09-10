// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NMCharacterMovementComponent.generated.h"

UCLASS()
class NEWMOON_API UNMCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UNMCharacterMovementComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void UpdateBasedMovement(float DeltaSeconds) override;

	void SetWalkSpeed();
	void SetCharacterMotion(ECharacterMotion NewCharacterMotion);
	void DoCrouch();
	void DoProne();

	FORCEINLINE ECharacterMotion GetCurrentCharacterMotion() const { return CurrentCharacterMotion; }

private:
	UFUNCTION()
	void OnRep_CurrentCharacterMotion(ECharacterMotion LastCharacterMotion);
	
public:
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Character Movement")
	bool bIsCrouched;
	
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Character Movement")
	bool bIsProne;
	
	float StandWalkSpeed;
	float CrouchWalkSpeed;
	float ProneWalkSpeed;

	UPROPERTY(Replicated)
	bool CrouchPlaying = false;
	
	UPROPERTY(Replicated)
	bool PronePlaying = false;
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentCharacterMotion, VisibleAnywhere, Category = "Character Movement")
	ECharacterMotion CurrentCharacterMotion;

	
};
