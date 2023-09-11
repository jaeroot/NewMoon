// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NMMountainDragon.h"

// Sets default values
ANMMountainDragon::ANMMountainDragon()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set Collision Capsule
	GetCapsuleComponent()->InitCapsuleSize(250, 250);
	
	// Set Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MountainDragon(TEXT("/Game/Assets/MountainDragon/Meshes/SK_MOUNTAIN_DRAGON.SK_MOUNTAIN_DRAGON"));
	if (SK_MountainDragon.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MountainDragon.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -250.0f), FRotator(0.0f, 0.0f, 0.0f));

	// Set Animation
	static ConstructorHelpers::FClassFinder<UAnimInstance> MountainDragon_Anim(TEXT("/Game/Blueprints/AI/MountainDragon/Animations/ABP_MountainDragon.ABP_MountainDragon_C"));
	if (MountainDragon_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MountainDragon_Anim.Class);
	}
}

void ANMMountainDragon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANMMountainDragon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANMMountainDragon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

