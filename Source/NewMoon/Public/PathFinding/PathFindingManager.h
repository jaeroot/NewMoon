// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathFindingManager.generated.h"

class UBoxComponent;

UENUM()
enum class EOctreeNum : uint16
{
	EON_TopUpperLeft = 0,
	EON_TopUpperRight,
	EON_TopLowerLeft,
	EON_TopLowerRight,
	EON_BottomUpperLeft,
	EON_BottomUpperRight,
	EON_BottomLowerLeft,
	EON_BottomLowerRight,

	EON_MAX
};

USTRUCT()
struct FOctreeNode
{
	GENERATED_BODY()

	UPROPERTY()
	FBox Box;

	UPROPERTY()
	TArray<AActor*> Items;

	TArray<FOctreeNode> Children;

	int Depth;

	FOctreeNode(const FBox& NewBox, int NewDepth) : Box(NewBox), Depth(NewDepth) {};

	FOctreeNode() : Box(FBox()), Depth(0) {};
};

UCLASS()
class NEWMOON_API APathFindingManager : public AActor
{
	GENERATED_BODY()
	
public:	
	APathFindingManager();
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
private:
	void ComposeOctree();
	bool AABB3D(const FBox& Box1, const FBox& Box2);
	void CheckCollision(FOctreeNode& Node, const FBox& Box1, const AActor* Actor);
	void AddChildren(FOctreeNode& Node);
	void DebugCollision(const FOctreeNode& Node);

public:	

private:
	UPROPERTY()
	UBoxComponent* Box;
	
	int32 MaxDepth;
	
	FOctreeNode Root;
};
