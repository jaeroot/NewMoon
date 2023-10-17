// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MountainDragon/NMMountainDragon.h"

#include "EngineUtils.h"
#include "AI/MountainDragon/NMMountainDragonAIController.h"
#include "AI/MountainDragon/NMMountainDragonAnimInstance.h"
#include "AI/MountainDragon/Gimmick/MapManageComponent.h"
#include "AI/MountainDragon/Gimmick/Tile.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/NMCharacter.h"
#include "Components/CombatComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ANMMountainDragon::ANMMountainDragon()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set Collision Capsule
	GetCapsuleComponent()->InitCapsuleSize(300, 300);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NMEnemy"));
	
	// Set Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MountainDragon(TEXT("/Game/Assets/MountainDragon/Meshes/SK_MOUNTAIN_DRAGON.SK_MOUNTAIN_DRAGON"));
	if (SK_MountainDragon.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MountainDragon.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(-80.0f, 0.0f, -300.0f), FRotator(0.0f, -90.0f, 0.0f));

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
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FlyAttack_Montage(TEXT("/Game/Blueprints/AI/MountainDragon/Animations/MountainDragonFlyAttack.MountainDragonFlyAttack"));
	if (FlyAttack_Montage.Succeeded())
	{
		FlyAttackMontage = FlyAttack_Montage.Object;
	}

	// Set AI Controller
	AIControllerClass = ANMMountainDragonAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Set Combat
	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);

	bIsBattleState = false;

	// Set Movement
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	// Set Default
	BaseLocation = FVector(-15000.0f, -3000.0f, 4300.0f);
	FlyLocation = FVector(-15000.0f, -1000.0f, 5300.0f);

	// Set TakeOffTimeline
	TakeOffTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TakeOffTimeline"));
	TakeOffTimelineFunction.BindUFunction(this, FName("ServerTakeOffInterp"));
	TakeOffTimelineFinish.BindUFunction(this, FName("ServerTakeOffFinish"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> TakeOff_Curve(TEXT("/Game/Blueprints/AI/MountainDragon/Animations/FlyCurve.FlyCurve"));
	if (TakeOff_Curve.Succeeded())
	{
		TakeOffCurve = TakeOff_Curve.Object;
		TakeOffTimeline->AddInterpFloat(TakeOffCurve, TakeOffTimelineFunction);
		TakeOffTimeline->SetTimelineFinishedFunc(TakeOffTimelineFinish);
	}
	TakeOffTimeline->SetLooping(false);

	// Set LandTimeline
	LandTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("LandTimeline"));
	LandTimelineFunction.BindUFunction(this, FName("ServerLandInterp"));
	LandTimelineFinish.BindUFunction(this, FName("ServerLandFinish"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Land_Curve(TEXT("/Game/Blueprints/AI/MountainDragon/Animations/LandCurve.LandCurve"));
	if (Land_Curve.Succeeded())
	{
		LandCurve = Land_Curve.Object;
		LandTimeline->AddInterpFloat(LandCurve, LandTimelineFunction);
		LandTimeline->SetTimelineFinishedFunc(LandTimelineFinish);
	}
	LandTimeline->SetLooping(false);

	// Set Decal
	static ConstructorHelpers::FObjectFinder<UMaterial> M_Decal(TEXT("/Game/Assets/Mesh/NewMaterial.NewMaterial"));
	if (M_Decal.Succeeded())
	{
		DecalMaterial = M_Decal.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterial> M_Decal2(TEXT("/Game/Assets/Mesh/Decal2.Decal2"));
	if (M_Decal2.Succeeded())
	{
		DecalMaterial2 = M_Decal2.Object;
	}

	// Set MapManager
	MapManager = CreateDefaultSubobject<UMapManageComponent>("MapManager");
	MapManager->SetIsReplicated(true);
}

void ANMMountainDragon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANMMountainDragon, bIsBattleState);
	DOREPLIFETIME(ANMMountainDragon, bGlide);
	DOREPLIFETIME(ANMMountainDragon, bFly);
}

void ANMMountainDragon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANMMountainDragon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (HasAuthority())
	{
		Combat->SetMaxHP(1000.0f);
		
		auto* NMAnim = Cast<UNMMountainDragonAnimInstance>(GetMesh()->GetAnimInstance());
		if (NMAnim)
		{
			NMAnim->OnAttack.AddUObject(this, &ANMMountainDragon::ServerDamage);
		}
	}
}

void ANMMountainDragon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetActorLocation().Z < 4300.0f)
	{
		SetActorLocation(BaseLocation);
	}
}

int ANMMountainDragon::GetHPPercentage()
{
	return (Combat->GetHP() / Combat->GetMaxHP()) * 100;
}

void ANMMountainDragon::ServerDamage_Implementation()
{
	auto* NMAIController = Cast<ANMMountainDragonAIController>(GetController());
	auto* Target = Cast<ANMCharacter>(NMAIController->GetBlackboardComponent()->GetValueAsObject(NMAIController->TargetKey));

	if (Target)
	{
		Target->Combat->TakeDamage(5.0f);
	}
}

void ANMMountainDragon::MulticastAttack_Implementation(int Rand)
{
	auto* NMAnim = Cast<UNMMountainDragonAnimInstance>(GetMesh()->GetAnimInstance());
	if (NMAnim)
	{
		NMAnim->Montage_Play(AttackMontage);

		switch (Rand)
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

void ANMMountainDragon::ServerTakeOff_Implementation()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	OldLocation = GetActorLocation();
	
	TakeOffTimeline->PlayFromStart();
}

void ANMMountainDragon::ServerTakeOffInterp_Implementation(float Value)
{
	const FVector NewLocation = FMath::Lerp(OldLocation, FlyLocation, Value);
	SetActorLocation(NewLocation);
}

void ANMMountainDragon::ServerTakeOffFinish_Implementation()
{
	TakeOffEnd.Broadcast();
}

void ANMMountainDragon::ServerLand_Implementation()
{
	OldLocation = GetActorLocation();
	LandTimeline->PlayFromStart();
}

void ANMMountainDragon::ServerLandInterp_Implementation(float Value)
{
	const FVector NewLocation = FMath::Lerp(BaseLocation, OldLocation, Value);
	SetActorLocation(NewLocation);
}

void ANMMountainDragon::ServerLandFinish_Implementation()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	LandEnd.Broadcast();
}

void ANMMountainDragon::ServerFireSpreadAttack_Implementation()
{
	MulticastFireSpreadAttack();

	// 타일 및 해머 생성
	MapManager->CreateTile(BaseLocation, 5, 5);
	MapManager->CreateHammer(BaseLocation, 5);

	// 기존 바닥 삭제
	for (TActorIterator<AStaticMeshActor> It(GetWorld()); It; ++It)
	{
		if (*It->GetName() == FString("SM_FloatingIsland_L_Island2"))
		{
			(*It)->Destroy();
		}
	}
}

void ANMMountainDragon::MulticastFireSpreadAttack_Implementation()
{
	auto* NMAnim = Cast<UNMMountainDragonAnimInstance>(GetMesh()->GetAnimInstance());
	if (NMAnim)
	{
		NMAnim->Montage_Play(FlyAttackMontage);
		NMAnim->Montage_JumpToSection(FName("FireSpreadAttack"));
	}
}

void ANMMountainDragon::ServerFireSpreadAttackFinished_Implementation()
{
	FireSpreadAttackEnd.Broadcast();
}

void ANMMountainDragon::ServerGlideAttack_Implementation()
{
	MulticastGlideAttack();

	FTimerHandle GlideAttackTimerHandle;
	float GlideAttackTime = 5.0f;
	GetWorld()->GetTimerManager().SetTimer(GlideAttackTimerHandle, FTimerDelegate::CreateLambda([&]() -> void
	{
		// check collision
		UWorld* World = GetWorld();
		FVector Center = FVector(BaseLocation.X, BaseLocation.Y, 4000.0f);
		FVector Size = FVector(750.0f, 1250.0f, 500.0f);

		if (World && HasAuthority())
		{
			TArray<FOverlapResult> OverlapResults;
			FCollisionQueryParams Params(NAME_None, false, this);
			bool bResult = World->OverlapMultiByChannel(
				OverlapResults,
				Center,
				FQuat::Identity,
				ECC_EnemyAttack,
				FCollisionShape::MakeBox(Size),
				Params
			);

			if (bResult)
			{
				for (auto OverlapResult : OverlapResults)
				{
					ANMCharacter* NMCharacter = Cast<ANMCharacter>(OverlapResult.GetActor());
					if (NMCharacter)
					{
						DrawDebugBox(World, Center, Size, FColor::Green, false, 3.5f);

						NMCharacter->Combat->TakeDamage(25.0f);
					}
				}
			}
			else
			{
				DrawDebugBox(World, Center, Size, FColor::Red, false, 3.5f);
			}
		}
		GlideAttackEnd.Broadcast();
		
		GetWorld()->GetTimerManager().ClearTimer(GlideAttackTimerHandle);
	}), GlideAttackTime, false);
}

void ANMMountainDragon::MulticastGlideAttack_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ADecalActor* DecalActor = World->SpawnActor<ADecalActor>(FVector(BaseLocation.X, BaseLocation.Y, 4000.0f), FRotator::ZeroRotator);

		if (DecalActor)
		{
			DecalActor->SetDecalMaterial(DecalMaterial);
			DecalActor->SetLifeSpan(5.0f);
			DecalActor->GetDecal()->DecalSize = FVector(1.0f, 1250.0f, 750.0f);
		}
	}
}

void ANMMountainDragon::ServerFireBallAttack_Implementation()
{
	// play Montage, draw attack range decal
	MulticastFireBallAttack();

	FTimerHandle FireBallAttackTimerHandle;
	float FireBallAttackTime = 10.0f;
	GetWorld()->GetTimerManager().SetTimer(FireBallAttackTimerHandle, FTimerDelegate::CreateLambda([&]() -> void
	{
		// check collision
		UWorld* World = GetWorld();
		FVector Center = FVector(BaseLocation.X, BaseLocation.Y, 4000.0f);
		float Radius = 2000.0f;

		float MaxDamge = 50.0f;
		float MinDamage = 0.0f;

		if (World && HasAuthority())
		{
			TArray<FOverlapResult> OverlapResults;
			bool bResult = World->OverlapMultiByChannel(
				OverlapResults,
				Center,
				FQuat::Identity,
				ECC_EnemyAttack,
				FCollisionShape::MakeSphere(Radius)
			);

			if (bResult)
			{
				for (auto OverlapResult : OverlapResults)
				{
					ANMCharacter* NMCharacter = Cast<ANMCharacter>(OverlapResult.GetActor());
					if (NMCharacter)
					{
						DrawDebugSphere(World, Center, Radius, 16, FColor::Green, false, 0.5f);

						float distance = (NMCharacter->GetActorLocation() - Center).Size();
						NMCharacter->Combat->TakeDamage(UKismetMathLibrary::MapRangeClamped(distance, 0.0f, Radius, MaxDamge, MinDamage));
					}
				}
			}
			else
			{
				DrawDebugSphere(World, Center, Radius, 16, FColor::Red, false, 0.5f);
			}
		}
		FireBallAttackEnd.Broadcast();

		GetWorld()->GetTimerManager().ClearTimer(FireBallAttackTimerHandle);
	}), FireBallAttackTime, false);
}

void ANMMountainDragon::MulticastFireBallAttack_Implementation()
{
	auto* NMAnim = Cast<UNMMountainDragonAnimInstance>(GetMesh()->GetAnimInstance());
	if (NMAnim)
	{
		NMAnim->Montage_Play(FlyAttackMontage);
		NMAnim->Montage_JumpToSection(FName("FireBallAttack"));
	}
	
	UWorld* World = GetWorld();
	if (World)
	{
		ADecalActor* DecalActor = World->SpawnActor<ADecalActor>(FVector(BaseLocation.X, BaseLocation.Y, 4000.0f), FRotator::ZeroRotator);

		if (DecalActor)
		{
			DecalActor->SetDecalMaterial(DecalMaterial2);
			DecalActor->SetLifeSpan(10.0f);
			DecalActor->GetDecal()->DecalSize = FVector(1.0f, 1800.0f, 1800.0f);
		}
	}
}