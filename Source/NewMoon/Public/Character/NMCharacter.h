// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "GameFramework/Character.h"
#include "NMCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class NEWMOON_API ANMCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANMCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	virtual void Jump() override;
	void RunButtonPressed();
	void RunButtonReleased();
	void CrouchButtonPressed();
	void ProneButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void AttackButtonPressed();
	void InteractButtonPressed();
public:	

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;
};
