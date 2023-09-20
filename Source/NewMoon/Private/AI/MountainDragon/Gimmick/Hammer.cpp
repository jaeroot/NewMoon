// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MountainDragon/Gimmick/Hammer.h"

AHammer::AHammer()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(160.0f, 110.0f);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(CapsuleComponent);

	Mesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh1"));
	Mesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh2"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Cylinder(TEXT("/Game/Assets/Mesh/SM_Cylinder.SM_Cylinder"));
	if (SM_Cylinder.Succeeded())
	{
		Mesh1->SetStaticMesh(SM_Cylinder.Object);
		Mesh1->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -130.0f), FRotator(-90.0f, 0.0f, 0.0f));
		Mesh1->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.5f));
		Mesh1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mesh1->SetupAttachment(RootComponent);
		
		Mesh2->SetStaticMesh(SM_Cylinder.Object);
		Mesh2->SetRelativeLocationAndRotation(FVector(-80.0f, 0.0f, 0.0f), FRotator(90.0f, 540.0f, 540.0f));
		Mesh2->SetRelativeScale3D(FVector(0.25f, 0.25f, 1.0f));
		Mesh2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mesh2->SetupAttachment(Mesh1);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> M_Basic(TEXT("/Game/Assets/Mesh/M_Basic.M_Basic"));
	if (M_Basic.Succeeded())
	{
		Mesh1->SetMaterial(0, M_Basic.Object);
		Mesh2->SetMaterial(0, M_Basic.Object);
	}
}

void AHammer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

