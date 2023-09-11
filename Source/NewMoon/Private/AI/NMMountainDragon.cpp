// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NMMountainDragon.h"

#include "AI/NMMountainDragonAIController.h"
#include "AI/NMMountainDragonAnimInstance.h"
#include "Components/CombatComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ANMMountainDragon::ANMMountainDragon()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set Collision Capsule
	GetCapsuleComponent()->InitCapsuleSize(300, 300);
	
	// Set Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MountainDragon(TEXT("/Game/Assets/MountainDragon/Meshes/SK_MOUNTAIN_DRAGON.SK_MOUNTAIN_DRAGON"));
	if (SK_MountainDragon.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MountainDragon.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(-80.0f, 0.0f, -300.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Overlap);

	// Set Animation
	static ConstructorHelpers::FClassFinder<UAnimInstance> MountainDragon_Anim(TEXT("/Game/Blueprints/AI/MountainDragon/Animations/ABP_MountainDragon.ABP_MountainDragon_C"));
	if (MountainDragon_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MountainDragon_Anim.Class);
	}

	// Set Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack_Montage(TEXT("/Game/Blueprints/AI/MountainDragon/Animations/MountainDragonAttack.MountainDragonAttack"));
	if (Attack_Montage.Succeeded())
	{
		AttackMontage = Attack_Montage.Object;
	}

	// Set AI Controller
	AIControllerClass = ANMMountainDragonAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Set Combat
	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);
	Combat->SetMaxHP(1000.0f);

	bIsBattleState = false;

	// Set Movement
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
}

void ANMMountainDragon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANMMountainDragon, bIsBattleState);
	DOREPLIFETIME(ANMMountainDragon, bGlideAttack);
	DOREPLIFETIME(ANMMountainDragon, bFireBallAttack);
	DOREPLIFETIME(ANMMountainDragon, bFireSpreadAttack);
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

int ANMMountainDragon::GetHPPercentage()
{
	Combat->TakeDamage(10);
	return (Combat->GetHP() / Combat->GetMaxHP()) * 100;
}

void ANMMountainDragon::Attack_Implementation()
{
	auto* NMAnim = Cast<UNMMountainDragonAnimInstance>(GetMesh()->GetAnimInstance());
	if (NMAnim)
	{
		NMAnim->Montage_Play(AttackMontage);

		int rand = FMath::RandRange(1, 4);
		switch (rand)
		{
		case 1:
			NMAnim->Montage_JumpToSection(FName("LeftClaw"));
			break;
		case 2:
			NMAnim->Montage_JumpToSection(FName("RightClaw"));
			break;
		case 3:
			NMAnim->Montage_JumpToSection(FName("Bite"));
			break;
		case 4:
			NMAnim->Montage_JumpToSection(FName("Share"));
		default:
			break;
		}
	}
}

