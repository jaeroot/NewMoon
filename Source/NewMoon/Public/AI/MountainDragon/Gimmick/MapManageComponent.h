// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "Components/ActorComponent.h"
#include "MapManageComponent.generated.h"

USTRUCT(BlueprintType)
struct FTileStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Actor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float X;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Y;

	FTileStruct() : Actor(nullptr), X(0), Y(0) {	}

	FTileStruct(AActor* NewActor, float NewX, float NewY) : Actor(NewActor), X(NewX), Y(NewY) {	}
};

USTRUCT(BlueprintType)
struct F2DTArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTileStruct> Array;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Dir;
	
	FTileStruct operator[] (int32 ArrayNum)
	{
		return Array[ArrayNum];
	}

	void Add(FTileStruct NewStruct)
	{
		Array.Add(NewStruct);
	}
};

USTRUCT(BlueprintType)
struct FHammerStruct
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Actor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Dir;

	FHammerStruct() : Actor(nullptr), Dir(0) { }

	FHammerStruct(AActor* NewActor, int Index) : Actor(NewActor), Dir(Index) { }
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWMOON_API UMapManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMapManageComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void CreateTile(FVector BaseLocation, int Size_X, int Size_Y);
	void CreateHammer(FVector BaseLocation, int Num);
	
protected:
	virtual void BeginPlay() override;

public:

private:
	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<F2DTArray> MapTile;
	
		
};
