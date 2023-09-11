// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "Net/UnrealNetwork.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, CurrentHP);
	DOREPLIFETIME(UCombatComponent, MaxHP);
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;
	
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
