// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "GameFramework/Actor.h"
#include "Hammer.generated.h"

UCLASS()
class NEWMOON_API AHammer : public AActor
{
	GENERATED_BODY()
	
public:	
	AHammer();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:

private:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* Mesh1;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* Mesh2;

};
