// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "Components/ItemInterface.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class NEWMOON_API AItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;
	
	virtual FItemData Interact() override;

	void UseItem();

	UFUNCTION(Server, Reliable)
	void ServerUseItem();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

public:
	UPROPERTY(BlueprintReadWrite)
	int ItemID;

	UPROPERTY(BlueprintReadWrite)
	FString Name;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* Thumbnail;

private:
	UPROPERTY(VisibleAnywhere, Category = "Item Properties")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, Category = "Item Properties")
	USphereComponent* AreaSphere;
};
