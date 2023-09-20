// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MountainDragon/Gimmick/Tile.h"

ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Tile(TEXT("/Game/Assets/Mesh/SM_Floor.SM_Floor"));
	if (SM_Tile.Succeeded())
	{
		Mesh->SetStaticMesh(SM_Tile.Object);
		Mesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	}
	RootComponent = Mesh;
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

