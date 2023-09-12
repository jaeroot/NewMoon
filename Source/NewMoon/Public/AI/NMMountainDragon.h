// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "NMMountainDragon.generated.h"

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

	FTileStruct()
		: Actor(nullptr), X(0), Y(0)
	{
		
	}

	FTileStruct(AActor* NewActor, float NewX, float NewY)
		: Actor(NewActor), X(NewX), Y(NewY)
	{
		
	}
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

	FHammerStruct(AActor* NewActor, int index) : Actor(NewActor), Dir(index) { }
};

UCLASS()
class NEWMOON_API ANMMountainDragon : public ACharacter
{
	GENERATED_BODY()

public:
	ANMMountainDragon();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	int GetHPPercentage();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttack(int Rand);
	
	UFUNCTION(Server, Reliable)
	void ServerDamage();
	
	UFUNCTION(NetMulticast, Reliable)
	void StartFly();
	
	UFUNCTION(NetMulticast, Reliable)
	void EndFly();
	
	UFUNCTION(NetMulticast, Reliable)
	void StartGlide();
	
	UFUNCTION(NetMulticast, Reliable)
	void GlideAttack();
	
	UFUNCTION(NetMulticast, Reliable)
	void EndGlide();

	UFUNCTION(NetMulticast, Reliable)
	void FireSpreadAttack();

	UFUNCTION(NetMulticast, Reliable)
	void FireBallAttack();
	
	UFUNCTION(NetMulticast, Reliable)
	void AddHammer(AActor* Actor, int input);
	
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

private:
	FOnTimelineFloat FlyTimelineFunction;
	FOnTimelineEvent FlyTimelineFinish;
	
	FOnTimelineFloat LandTimelineFunction;
	FOnTimelineEvent LandTimelineFinish;
	
	FOnTimelineFloat HammerTimelineFunction;
	FOnTimelineEvent HammerTimelineFinish;
	
	FOnTimelineFloat TileTimelineFunction;
	FOnTimelineEvent TileTimelineFinish;
	
	UFUNCTION(NetMulticast, Reliable)
	void FlyInterp(float Value);
	UFUNCTION(NetMulticast, Reliable)
	void FlyFinish();
	
	UFUNCTION(NetMulticast, Reliable)
	void LandInterp(float Value);
	UFUNCTION(NetMulticast, Reliable)
	void LandFinish();
	
	UFUNCTION(NetMulticast, Reliable)
	void HammerInterp(float Value);
	UFUNCTION(NetMulticast, Reliable)
	void HammerFinish();
	
	UFUNCTION(NetMulticast, Reliable)
	void TileInterp(float Value);
	UFUNCTION(NetMulticast, Reliable)
	void TileFinish();

public:	
	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;
	
	UPROPERTY(Replicated)
	bool bIsBattleState;
	
	UPROPERTY(Replicated)
	bool bGlideAttack;
	UPROPERTY(Replicated)
	bool bFly;
	UPROPERTY(Replicated)
	bool bFireBallAttack;
	UPROPERTY(Replicated)
	bool bFireSpreadAttack;

	FVector BaseLocation;
	FVector FlyLocation;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UTimelineComponent* FlyTimeline;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UTimelineComponent* LandTimeline;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UTimelineComponent* HammerTimeline;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UTimelineComponent* TileTimeline;

private:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AttackMontage;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UCurveFloat* FlyCurve;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UCurveFloat* LandCurve;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UCurveFloat* HammerCurve;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UCurveFloat* TileCurve;

	FVector OldLocation;

	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<F2DTArray> MapTile;

	// UPROPERTY(Replicated, VisibleAnywhere)
	// TArray<FHammerStruct> Hammer;

	// UPROPERTY(Replicated)
	// TArray<int> HammerDir;

	UPROPERTY(VisibleAnywhere)
	UMaterial* DecalMaterial;
	
	UPROPERTY(VisibleAnywhere)
	UMaterial* DecalMaterial2;

	bool FlyFireAttack = false;
};
