// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "Components/ActorComponent.h"
#include "Item/Item.h"
#include "InventoryComponent.generated.h"

struct FItemData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWMOON_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddToInventory(FItemData NewItem);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(Replicated, BlueprintReadWrite)
	TArray<FItemData> Inventory;
};
