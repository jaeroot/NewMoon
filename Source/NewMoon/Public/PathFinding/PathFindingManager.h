// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathFindingManager.generated.h"

class UBoxComponent;

/*
 *** DEPRECATED ***
 * 해당 기능은 Old 버전이며 현재는 사용하지 않음
 * Nav3dSystem 프로젝트에서 별도로 개발중이며 추후 플러그인 형태로 사용할 예정
 */

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
	void DrawDebugOctreeBox(const FOctreeNode& Node);

	void BroadPhase();

public:	

private:
	UPROPERTY()
	UBoxComponent* Box;

	UPROPERTY()
	TArray<AActor*> CollisionItems;
	
	int32 MaxDepth;
	
	FOctreeNode Root;
};
