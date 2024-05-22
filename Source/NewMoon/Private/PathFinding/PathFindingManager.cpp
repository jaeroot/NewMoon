// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinding/PathFindingManager.h"

#include "EngineUtils.h"
#include "NewMoon.h"

APathFindingManager::APathFindingManager()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);

	MaxDepth = 10;
}

void APathFindingManager::BeginPlay()
{
	Super::BeginPlay();

	
	// Root = FOctreeNode(GetComponentsBoundingBox(), 0);
	// ComposeOctree();
	//
	// DebugCollision(Root);
	// DrawDebugOctreeBox(Root);
}

void APathFindingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

void APathFindingManager::ComposeOctree()
{
	FBox ActorBoundingBox = GetComponentsBoundingBox();
	
	for(TActorIterator<AActor> ActorIT(GetWorld()); ActorIT; ++ActorIT)
	{
		if (*ActorIT == this)
		{
			continue;
		}
		
		// Broad Phase Collision detection
		if (AABB3D(ActorBoundingBox, ActorIT->GetComponentsBoundingBox()))
		{
			NMLOG(Warning, TEXT("%s"), *ActorIT->GetName());
			CheckCollision(Root, ActorBoundingBox, *ActorIT);
		}
	}
}

bool APathFindingManager::AABB3D(const FBox& Box1, const FBox& Box2)
{
	FVector Min1 = Box1.Min;
	FVector Max1 = Box1.Max;

	FVector Min2 = Box2.Min;
	FVector Max2 = Box2.Max;

	if (Min1.X > Max2.X || Max1.X < Min2.X)
	{
		return false;
	}
	if (Min1.Y > Max2.Y || Max1.Y < Min2.Y)
	{
		return false;
	}
	if (Min1.Z > Max2.Z || Max1.Z < Min2.Z)
	{
		return false;
	}

	return true;
}

void APathFindingManager::CheckCollision(FOctreeNode& Node, const FBox& Box1, const AActor* Actor)
{
	FVector Center = Box1.GetCenter();
	FBox Box2 = Actor->GetComponentsBoundingBox();

	TArray<EOctreeNum> CollisionList;
	bool bLessX = Center.X >= Box2.Min.X;
	bool bLessY = Center.Y >= Box2.Min.Y;
	bool bLessZ = Center.Z >= Box2.Min.Z;
	bool bGreaterX = Center.X <= Box2.Max.X;
	bool bGreaterY = Center.Y <= Box2.Max.Y;
	bool bGreaterZ = Center.Z <= Box2.Max.Z;

	if (bLessX && bLessY && bLessZ)
	{
		CollisionList.Add(EOctreeNum::EON_BottomLowerLeft);
	}

	if (bLessX && bLessY && bGreaterZ)
	{
		CollisionList.Add(EOctreeNum::EON_TopLowerLeft);
	}

	if (bLessX && bGreaterY && bLessZ)
	{
		CollisionList.Add(EOctreeNum::EON_BottomUpperLeft);
	}

	if (bLessX && bGreaterY && bGreaterZ)
	{
		CollisionList.Add(EOctreeNum::EON_TopUpperLeft);
	}
	
	if (bGreaterX && bLessY && bLessZ)
	{
		CollisionList.Add(EOctreeNum::EON_BottomLowerRight);
	}

	if (bGreaterX && bLessY && bGreaterZ)
	{
		CollisionList.Add(EOctreeNum::EON_TopLowerRight);
	}

	if (bGreaterX && bGreaterY && bLessZ)
	{
		CollisionList.Add(EOctreeNum::EON_BottomUpperRight);
	}

	if (bGreaterX && bGreaterY && bGreaterZ)
	{
		CollisionList.Add(EOctreeNum::EON_TopUpperRight);
	}

	
	if (CollisionList.Num() == 1 && Node.Depth <= MaxDepth)
	{
		if (Node.Children.IsEmpty())
		{
			AddChildren(Node);
		}

		CheckCollision(Node.Children[static_cast<uint16>(CollisionList[0])], Node.Children[static_cast<uint16>(CollisionList[0])].Box, Actor);
	}
	else
	{
		Node.Items.Add(const_cast<TArray<AActor*>::ElementType>(Actor));
	}
}

void APathFindingManager::AddChildren(FOctreeNode& Node)
{
	FVector Center = Node.Box.GetCenter();
	FVector Min = Node.Box.Min;
	FVector Max = Node.Box.Max;
	
	Node.Children.Add(FOctreeNode(FBox(FVector(Min.X, Center.Y, Center.Z), FVector(Center.X, Max.Y, Max.Z)), Node.Depth + 1));
	Node.Children.Add(FOctreeNode(FBox(FVector(Center.X, Center.Y, Center.Z), FVector(Max.X, Max.Y, Max.Z)), Node.Depth + 1));
	Node.Children.Add(FOctreeNode(FBox(FVector(Min.X, Min.Y, Center.Z), FVector(Center.X, Center.Y, Max.Z)), Node.Depth + 1));
	Node.Children.Add(FOctreeNode(FBox(FVector(Center.X, Min.Y, Center.Z), FVector(Max.X, Center.Y, Max.Z)), Node.Depth + 1));
	Node.Children.Add(FOctreeNode(FBox(FVector(Min.X, Center.Y, Min.Z), FVector(Center.X, Max.Y, Center.Z)), Node.Depth + 1));
	Node.Children.Add(FOctreeNode(FBox(FVector(Center.X, Center.Y, Min.Z), FVector(Max.X, Max.Y, Center.Z)), Node.Depth + 1));
	Node.Children.Add(FOctreeNode(FBox(FVector(Min.X, Min.Y, Min.Z), FVector(Center.X, Center.Y, Center.Z)), Node.Depth + 1));
	Node.Children.Add(FOctreeNode(FBox(FVector(Center.X, Min.Y, Min.Z), FVector(Max.X, Center.Y, Center.Z)), Node.Depth + 1));
}

void APathFindingManager::DebugCollision(const FOctreeNode& Node)
{
	if (!Node.Items.IsEmpty())
	{
		NMLOG(Warning, TEXT("depth: %d"), Node.Depth);
		
		for (auto i : Node.Items)
		{
			NMLOG(Warning, TEXT("%s"), *i->GetName());
		}
	}

	if (!Node.Children.IsEmpty())
	{
		for (auto n : Node.Children)
		{
			DebugCollision(n);
		}
	}
}

void APathFindingManager::DrawDebugOctreeBox(const FOctreeNode& Node)
{
	if (!Node.Children.IsEmpty())
	{
		for (auto c : Node.Children)
		{
			DrawDebugOctreeBox(c);
		}
	}
	else
	{
		if (Node.Items.IsEmpty())
		{
			DrawDebugBox(GetWorld(), Node.Box.GetCenter(), Node.Box.GetExtent(), FColor::Green, false, 10.0f);
		}
		else
		{
			DrawDebugBox(GetWorld(), Node.Box.GetCenter(), Node.Box.GetExtent(), FColor::Red, false, 10.0f);
		}
			
	}
	
}

void APathFindingManager::BroadPhase()
{
	FBox ActorBoundingBox = GetComponentsBoundingBox();
	
	for(TActorIterator<AActor> ActorIT(GetWorld()); ActorIT; ++ActorIT)
	{
		if (*ActorIT == this)
		{
			continue;
		}
		
		// Broad Phase Collision detection
		if (AABB3D(ActorBoundingBox, ActorIT->GetComponentsBoundingBox()))
		{
			CollisionItems.Add(*ActorIT);
		}
	}
}
