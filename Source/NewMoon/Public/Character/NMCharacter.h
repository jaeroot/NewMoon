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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetOverlappingWeapon(class AWeapon* Weapon);
	void SetOverlappingActor(AActor* Actor);
	
	bool CheckCanStand(float HalfHeight);
	
	UFUNCTION(BlueprintCallable)
	bool IsWeaponEquipped();

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
	
	UFUNCTION(BlueprintCallable)
	float GetHPPercentage();

private:
	UFUNCTION(Server, Reliable)
	void ServerCrouchButtonPressed();
	
	UFUNCTION(Server, Reliable)
	void ServerProneButtonPressed();

	UFUNCTION(Server, Reliable)
	void ServerInteractButtonPressed(bool Item);
	
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
	
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);
	
	UFUNCTION()
	void OnRep_OverlappingActor(AActor* LastActor);
	
	
public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UTimelineComponent* CrouchTimeline;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UTimelineComponent* ProneTimeline;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UTimelineComponent* CameraRotTimeline;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> HPBarWidgetClass;
	
	UPROPERTY()
	class UUserWidget* HPBarWidget;
	
	float StandHalfHeight;
	float CrouchHalfHeight;
	float ProneHalfHeight;
	float Radius;
	
	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UInventoryComponent* Inventory;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UCurveFloat* CrouchCurve;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UCurveFloat* ProneCurve;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UCurveFloat* CameraRotCurve;
	
	bool ControlRotationBlocked;
	FRotator ControlRotation;
	
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;
	
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingActor)
	AActor* OverlappingActor;

};
