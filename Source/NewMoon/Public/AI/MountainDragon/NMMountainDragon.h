// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "NMMountainDragon.generated.h"

DECLARE_MULTICAST_DELEGATE(FTakeOffEndDelegate);
DECLARE_MULTICAST_DELEGATE(FLandEndDelegate);
DECLARE_MULTICAST_DELEGATE(FFireSpreadAttackFinished);
DECLARE_MULTICAST_DELEGATE(FFireBallAttackFinished);
DECLARE_MULTICAST_DELEGATE(FGlideAttackFinished);

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

	UFUNCTION(Server, Reliable)
	void ServerTakeOff();
	UFUNCTION(Server, Reliable)
	void ServerLand();
	
	FTakeOffEndDelegate TakeOffEnd;
	FLandEndDelegate LandEnd;
	FFireSpreadAttackFinished FireSpreadAttackEnd;
	FFireBallAttackFinished FireBallAttackEnd;
	FGlideAttackFinished GlideAttackEnd;

	UFUNCTION(Server, Reliable)
	void ServerFireSpreadAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFireSpreadAttack();
	UFUNCTION(Server, Reliable)
	void ServerFireSpreadAttackFinished();
	
	UFUNCTION(Server, Reliable)
	void ServerGlideAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastGlideAttack();
	
	UFUNCTION(Server, Reliable)
	void ServerFireBallAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFireBallAttack();

	void SetFly(bool NewFly) { bFly = NewFly; }
	void SetGlide(bool NewGlide) { bGlide = NewGlide; }
	
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

public:	
	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;
	
	UPROPERTY(Replicated)
	bool bIsBattleState;
	UPROPERTY(Replicated)
	bool bGlide;
	UPROPERTY(Replicated)
	bool bFly;
	
	FVector BaseLocation;
	FVector FlyLocation;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UTimelineComponent* TakeOffTimeline;
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UTimelineComponent* LandTimeline;

private:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AttackMontage;
	
	UPROPERTY(EditAnywhere)
	class UAnimMontage* FlyAttackMontage;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UCurveFloat* TakeOffCurve;
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UCurveFloat* LandCurve;

	FVector OldLocation;

	UPROPERTY(VisibleAnywhere)
	UMaterial* DecalMaterial;
	
	UPROPERTY(VisibleAnywhere)
	UMaterial* DecalMaterial2;

	class UMapManageComponent* MapManager;
};
