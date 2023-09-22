// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "NMMountainDragon.generated.h"

DECLARE_MULTICAST_DELEGATE(FTakeOffEndDelegate);
DECLARE_MULTICAST_DELEGATE(FLandEndDelegate);

UCLASS()
class NEWMOON_API ANMMountainDragon : public ACharacter
{
	GENERATED_BODY()

public:
	ANMMountainDragon();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	int GetHPPercentage();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttack(int Rand);
	
	UFUNCTION(Server, Reliable)
	void ServerDamage();
	
	UFUNCTION(NetMulticast, Reliable)
	void EndFly();

	UFUNCTION(Server, Reliable)
	void ServerTakeOff();
	
	UFUNCTION(Server, Reliable)
	void ServerLand();
	FTakeOffEndDelegate TakeOffEnd;
	FLandEndDelegate LandEnd;
	
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

	void SetFly(bool NewFly) { bFly = NewFly; }
	void SetGlide(bool NewGlide) { bGlide = NewGlide; }
	
	//
	// UFUNCTION(NetMulticast, Reliable)
	// void AddHammer(AActor* Actor, int input);
	
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

private:
	FOnTimelineFloat TakeOffTimelineFunction;
	FOnTimelineEvent TakeOffTimelineFinish;
	
	FOnTimelineFloat LandTimelineFunction;
	FOnTimelineEvent LandTimelineFinish;
	
	FOnTimelineFloat HammerTimelineFunction;
	FOnTimelineEvent HammerTimelineFinish;
	
	FOnTimelineFloat TileTimelineFunction;
	FOnTimelineEvent TileTimelineFinish;

	UFUNCTION(Server, Reliable)
	void ServerTakeOffInterp(float Value);
	UFUNCTION(Server, Reliable)
	void ServerTakeOffFinish();
	
	UFUNCTION(Server, Reliable)
	void ServerLandInterp(float Value);
	UFUNCTION(Server, Reliable)
	void ServerLandFinish();
	
	// UFUNCTION(NetMulticast, Reliable)
	// void HammerInterp(float Value);
	// UFUNCTION(NetMulticast, Reliable)
	// void HammerFinish();
	//
	// UFUNCTION(NetMulticast, Reliable)
	// void TileInterp(float Value);
	// UFUNCTION(NetMulticast, Reliable)
	// void TileFinish();

public:	
	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;
	
	UPROPERTY(Replicated)
	bool bIsBattleState;
	
	UPROPERTY(Replicated)
	bool bGlide;
	UPROPERTY(Replicated)
	bool bFly;
	// UPROPERTY(Replicated)
	// bool bFireBallAttack;
	// UPROPERTY(Replicated)
	// bool bFireSpreadAttack;

	FVector BaseLocation;
	FVector FlyLocation;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UTimelineComponent* TakeOffTimeline;
	
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
	UCurveFloat* TakeOffCurve;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UCurveFloat* LandCurve;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UCurveFloat* HammerCurve;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UCurveFloat* TileCurve;

	FVector OldLocation;

	// UPROPERTY(Replicated, VisibleAnywhere)
	// TArray<FHammerStruct> Hammer;

	// UPROPERTY(Replicated)
	// TArray<int> HammerDir;

	UPROPERTY(VisibleAnywhere)
	UMaterial* DecalMaterial;
	
	UPROPERTY(VisibleAnywhere)
	UMaterial* DecalMaterial2;

	bool FlyFireAttack = false;

	class UMapManageComponent* MapManager;
};
