// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MountainDragon/Gimmick/MapManageComponent.h"

#include "AI/MountainDragon/NMMountainDragon.h"
#include "AI/MountainDragon/Gimmick/Hammer.h"
#include "AI/MountainDragon/Gimmick/Tile.h"
#include "Net/UnrealNetwork.h"
#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"

UMapManageComponent::UMapManageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Tile Movement Timeline
	TileMoveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TileMoveTimeline"));
	TileMoveTimelineFunction.BindUFunction(this, FName("MulticastTileMoveInterp"));
	TileMoveTimelineFinish.BindUFunction(this, FName("ServerTileMoveFinish"));
	
	static ConstructorHelpers::FObjectFinder<UCurveFloat> Tile_Curve(TEXT("/Game/Blueprints/AI/MountainDragon/Animations/TileCurve.TileCurve"));
	if (Tile_Curve.Succeeded())
	{
		TileMoveTimelineCurve = Tile_Curve.Object;
		TileMoveTimeline->AddInterpFloat(TileMoveTimelineCurve, TileMoveTimelineFunction);
		TileMoveTimeline->SetTimelineFinishedFunc(TileMoveTimelineFinish);
	}
	TileMoveTimeline->SetLooping(false);

	// Hammer Rotation Timeline
	HammerRotateTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("HammerRotateTimeline"));
	HammerRotateTimelineFunction.BindUFunction(this, FName("MulticastHammerRotateInterp"));
	HammerRotateTimelineFinish.BindUFunction(this, FName("ServerHammerRotateFinish"));

	static ConstructorHelpers::FObjectFinder<UCurveVector> Hammer_Curve(TEXT("/Game/Blueprints/AI/MountainDragon/Animations/HammerCurve.HammerCurve"));
	if (Hammer_Curve.Succeeded())
	{
		HammerRotateTimelineCurve = Hammer_Curve.Object;
		HammerRotateTimeline->AddInterpVector(HammerRotateTimelineCurve, HammerRotateTimelineFunction);
		HammerRotateTimeline->SetTimelineFinishedFunc(HammerRotateTimelineFinish);
	}
	HammerRotateTimeline->SetLooping(false);
}

void UMapManageComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMapManageComponent, MapTile);
	DOREPLIFETIME(UMapManageComponent, Hammer);
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

void UMapManageComponent::MulticastTileMoveInterp_Implementation(float Value)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			FVector NewLocation;
			NewLocation.X = FMath::Lerp(MapTile[i][j].X, MapTile[i][j].X - (MapTile[i].Dir * 500), Value);
			NewLocation.Y = MapTile[i][j].Actor->GetActorLocation().Y;
			NewLocation.Z = MapTile[i][j].Actor->GetActorLocation().Z;
			
			MapTile[i][j].Actor->SetActorLocation(NewLocation);
		}
	}
}

void UMapManageComponent::ServerTileMoveFinish_Implementation()
{
	for (auto hammer : Hammer)
	{
		hammer.Actor->Destroy();
	}

	auto character = Cast<ANMMountainDragon>(GetOwner());
	if (character)
	{
		character->ServerFireSpreadAttackFinished();
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
			
			Hammer.Add(FHammerStruct(World->SpawnActor<AHammer>(AHammer::StaticClass(), Loc, Rot), MapTile[i].Dir));
		}
	}

	HammerRotateTimeline->PlayFromStart();
}

void UMapManageComponent::MulticastHammerRotateInterp_Implementation(FVector Value)
{
	for (auto hammer : Hammer)
	{
		FRotator Rot = FRotator(150 * hammer.Dir * Value.X, Value.Y, Value.Z);

		hammer.Actor->SetActorRotation(Rot);
	}
}

void UMapManageComponent::ServerHammerRotateFinish_Implementation()
{
	TileMoveTimeline->PlayFromStart();
}

