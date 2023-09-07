// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NMCharacter.h"

ANMCharacter::ANMCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ANMCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ANMCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &ANMCharacter::RunButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &ANMCharacter::RunButtonReleased);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ANMCharacter::CrouchButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Prone"), IE_Pressed, this, &ANMCharacter::ProneButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &ANMCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &ANMCharacter::AimButtonReleased);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &ANMCharacter::AttackButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ANMCharacter::InteractButtonPressed);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ANMCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ANMCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ANMCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ANMCharacter::LookUp);
}

void ANMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANMCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void ANMCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void ANMCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ANMCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ANMCharacter::Jump()
{
	Super::Jump();
}

void ANMCharacter::RunButtonPressed()
{
}

void ANMCharacter::RunButtonReleased()
{
}

void ANMCharacter::CrouchButtonPressed()
{
}

void ANMCharacter::ProneButtonPressed()
{
}

void ANMCharacter::AimButtonPressed()
{
}

void ANMCharacter::AimButtonReleased()
{
}

void ANMCharacter::AttackButtonPressed()
{
}

void ANMCharacter::InteractButtonPressed()
{
}

