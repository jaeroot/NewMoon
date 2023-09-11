// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class NEWMOON_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATile();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* Mesh;

};
