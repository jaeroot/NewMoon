// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWMOON_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	friend class ANMCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE float GetHP() const { return CurrentHP; }

	UFUNCTION(Server, Reliable)
	void TakeDamage(float Damage);

	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	void SetMaxHP(float Value) { MaxHP = Value; CurrentHP = MaxHP; }
	
	void EquipWeapon(AWeapon* WeaponToEquip);
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnRep_EquippedWeapon();

private:
	UPROPERTY(Replicated)
	float CurrentHP;

	UPROPERTY(Replicated)
	float MaxHP;

	ANMCharacter* NMCharacter;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;
		
};
