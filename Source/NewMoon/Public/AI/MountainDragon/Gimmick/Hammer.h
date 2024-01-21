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

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSync(FRotator NewRot, float NewDir);

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

	float TotalTime = 0.0f;
	float ClientTime = 0.0f;
	float Speed = 1.0f;
	float Direction = 1.0f;
};
