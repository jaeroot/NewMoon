// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "GameFramework/Character.h"
#include "NMMountainDragon.generated.h"

UCLASS()
class NEWMOON_API ANMMountainDragon : public ACharacter
{
	GENERATED_BODY()

public:
	ANMMountainDragon();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

public:	

};
