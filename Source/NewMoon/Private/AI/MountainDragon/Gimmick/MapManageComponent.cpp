// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MountainDragon/Gimmick/MapManageComponent.h"

#include "AI/MountainDragon/Gimmick/Tile.h"
#include "Net/UnrealNetwork.h"

UMapManageComponent::UMapManageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UMapManageComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMapManageComponent, MapTile);
}

void UMapManageComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UMapManageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UMapManageComponent::CreateTile(FVector BaseLocation, int Size_X, int Size_Y)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		for (int y = 0; y < Size_Y; y++)
		{
			MapTile.Add(F2DTArray());
			MapTile[y].Dir = FMath::RandBool() ? 1 : -1;

			for (int x = 0; x < Size_X; x++)
			{
				FVector Loc;
				Loc.X = BaseLocation.X + 250.0f * (2 * x - (Size_X - 1));
				Loc.Y = BaseLocation.Y + 250.0f * (2 * y - (Size_Y - 1));
				Loc.Z = 4000.0f;

				MapTile[y].Add(FTileStruct(World->SpawnActor<ATile>(ATile::StaticClass(), Loc, FRotator::ZeroRotator), Loc.X, Loc.Y));
			}
		}
	}
}

void UMapManageComponent::CreateHammer(FVector BaseLocation, int Num)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		for (int i = 0; i < Num; i++)
		{
			FVector Loc;
			Loc.X = BaseLocation.X + (MapTile[i].Dir * 1400.0f);
			Loc.Y = BaseLocation.Y + 250.0f * (2 * i - (Num - 1));
			Loc.Z = 4100.0f;

			FRotator Rot = FRotator(MapTile[i].Dir * 150.0f, 0.0f, 0.0f);

			UObject* Object = StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("/Game/Assets/Mesh/hammer.hammer"));
			UBlueprint* BP = Cast<UBlueprint>(Object);
			TSubclassOf<class UObject> BP_Hammer = static_cast<UClass*>(BP->GeneratedClass);
			World->SpawnActor<AActor>(BP_Hammer, Loc, Rot);
		}
	}
}

