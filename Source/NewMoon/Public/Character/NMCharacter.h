// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "NMCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class NEWMOON_API ANMCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANMCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	bool CheckCanStand(float HalfHeight);

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	virtual void Jump() override;
	void RunButtonPressed();
	void RunButtonReleased();
	void CrouchButtonPressed();
	void ProneButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void AttackButtonPressed();
	void InteractButtonPressed();
	void ControlRotationButtonPressed();
	void ControlRotationButtonReleased();

private:
	UFUNCTION(Server, Reliable)
	void ServerCrouchButtonPressed();
	
	UFUNCTION(Server, Reliable)
	void ServerProneButtonPressed();
	
	FOnTimelineFloat CrouchTimelineFunction;
	FOnTimelineEvent CrouchTimelineFinish;

	FOnTimelineFloat ProneTimelineFunction;
	FOnTimelineEvent ProneTimelineFinish;

	FOnTimelineFloat CameraRotTimelineFunction;
	FOnTimelineEvent CameraRotTimelineFinish;
	
	UFUNCTION(NetMulticast, Reliable)
	void CrouchInterp(float Value);

	UFUNCTION(NetMulticast, Reliable)
	void CrouchFinish();
	
	UFUNCTION(NetMulticast, Reliable)
	void ProneInterp(float Value);

	UFUNCTION(NetMulticast, Reliable)
	void ProneFinish();

	UFUNCTION()
	void CameraRotInterp(float Value);

	UFUNCTION()
	void CameraRotFinish();
	
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UTimelineComponent* CrouchTimeline;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UTimelineComponent* ProneTimeline;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UTimelineComponent* CameraRotTimeline;
	
	float StandHalfHeight;
	float CrouchHalfHeight;
	float ProneHalfHeight;
	float Radius;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UCurveFloat* CrouchCurve;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UCurveFloat* ProneCurve;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UCurveFloat* CameraRotCurve;
	
	bool ControlRotationBlocked;
	FRotator ControlRotation;
	

};
