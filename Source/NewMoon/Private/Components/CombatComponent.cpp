// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "Character/NMCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxHP = 500.0f;
	CurrentHP = 500.0f;
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, CurrentHP);
	DOREPLIFETIME(UCombatComponent, MaxHP);
	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
}

void UCombatComponent::TakeDamage_Implementation(float Damage)
{
	CurrentHP -= Damage;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (NMCharacter == nullptr || WeaponToEquip == nullptr)
	{
		return;
	}

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

	const USkeletalMeshSocket* HandSocket = NMCharacter->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, NMCharacter->GetMesh());
	}
	EquippedWeapon->SetOwner(NMCharacter);
	
	NMCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	NMCharacter->bUseControllerRotationYaw = true;
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && NMCharacter)
	{
		NMCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		NMCharacter->bUseControllerRotationYaw = true;
	}
}