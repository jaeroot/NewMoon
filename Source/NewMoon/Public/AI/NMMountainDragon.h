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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	int GetHPPercentage();

	UFUNCTION(NetMulticast, Reliable)
	void Attack();
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(Replicated)
	bool bIsBattleState;
	
	UPROPERTY(Replicated)
	bool bGlideAttack;
	UPROPERTY(Replicated)
	bool bFireBallAttack;
	UPROPERTY(Replicated)
	bool bFireSpreadAttack;

private:
	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AttackMontage;

};
