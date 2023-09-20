// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MountainDragon/NMMountainDragon.h"

#include "EngineUtils.h"
#include "AI/MountainDragon/NMMountainDragonAIController.h"
#include "AI/MountainDragon/NMMountainDragonAnimInstance.h"
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
	LandTimelineFunction.BindUFunction(this, FName("LandInterp"));
	LandTimelineFinish.BindUFunction(this, FName("LandFinish"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Land_Curve(TEXT("/Game/Blueprints/AI/MountainDragon/Animations/LandCurve.LandCurve"));
	if (Land_Curve.Succeeded())
	{
		LandCurve = Land_Curve.Object;
		LandTimeline->AddInterpFloat(LandCurve, LandTimelineFunction);
		LandTimeline->SetTimelineFinishedFunc(LandTimelineFinish);
	}
	LandTimeline->SetLooping(false);
	
	// Set Hammer Timeline
	HammerTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("HammerTimeline"));
	HammerTimelineFunction.BindUFunction(this, FName("HammerInterp"));
	HammerTimelineFinish.BindUFunction(this, FName("HammerFinish"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Hammer_Curve(TEXT("/Game/Blueprints/AI/MountainDragon/Animations/HammerCurve.HammerCurve"));
	if (Hammer_Curve.Succeeded())
	{
		HammerCurve = Hammer_Curve.Object;
		HammerTimeline->AddInterpFloat(HammerCurve, HammerTimelineFunction);
		HammerTimeline->SetTimelineFinishedFunc(HammerTimelineFinish);
	}
	HammerTimeline->SetLooping(false);
	
	// Set Tile Timeline
	TileTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TileTimeline"));
	TileTimelineFunction.BindUFunction(this, FName("TileInterp"));
	TileTimelineFinish.BindUFunction(this, FName("TileFinish"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Tile_Curve(TEXT("/Game/Blueprints/AI/MountainDragon/Animations/TileCurve.TileCurve"));
	if (Tile_Curve.Succeeded())
	{
		TileCurve = Tile_Curve.Object;
		TileTimeline->AddInterpFloat(TileCurve, TileTimelineFunction);
		TileTimeline->SetTimelineFinishedFunc(TileTimelineFinish);
	}
	TileTimeline->SetLooping(false);

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
}

void ANMMountainDragon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANMMountainDragon, bIsBattleState);
	DOREPLIFETIME(ANMMountainDragon, bGlideAttack);
	DOREPLIFETIME(ANMMountainDragon, bFly);
	DOREPLIFETIME(ANMMountainDragon, bFireBallAttack);
	DOREPLIFETIME(ANMMountainDragon, bFireSpreadAttack);
	DOREPLIFETIME(ANMMountainDragon, MapTile);
	// DOREPLIFETIME(ANMMountainDragon, Hammer);
	// DOREPLIFETIME(ANMMountainDragon, HammerDir);
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
	
	// if (HasAuthority())
	// {
	// 	TakeOffEnd.Broadcast();
	// 	if (bFly)
	// 	{
	// 		if (!FlyFireAttack)
	// 			FireSpreadAttack();
	// 		else
	// 			FireBallAttack();
	// 	}
	// 	else if (bGlideAttack)
	// 	{
	// 		GetCharacterMovement()->MaxFlySpeed = 1800.0f;
	// 	}
	// }
}

void ANMMountainDragon::EndFly_Implementation()
{
	if (FlyFireAttack)
	{
		// check collision
		UWorld* World = GetWorld();
		FVector Center = FVector(BaseLocation.X, BaseLocation.Y, 4000.0f);
		float Radius = 2000.0f;

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
						NMCharacter->Combat->TakeDamage(UKismetMathLibrary::MapRangeClamped(distance, 0, 2000, 50, 0));
					}
				}
			}
			else
			{
				DrawDebugSphere(World, Center, Radius, 16, FColor::Red, false, 0.5f);
			}
		}
	}
	
	
	bFireSpreadAttack = false;
	bFireBallAttack = false;

	OldLocation = GetActorLocation();
	LandTimeline->PlayFromStart();
}

void ANMMountainDragon::StartGlide_Implementation()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	bGlideAttack = true;
	
	OldLocation = GetActorLocation();
	TakeOffTimeline->PlayFromStart();
}

void ANMMountainDragon::GlideAttack_Implementation()
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

void ANMMountainDragon::EndGlide_Implementation()
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
	
	
	OldLocation = GetActorLocation();
	LandTimeline->PlayFromStart();
}

void ANMMountainDragon::FireSpreadAttack_Implementation()
{
	bFireSpreadAttack = true;

	if (!HasAuthority())
		return;

	// 타일, 망치 생성
	UWorld* World = this->GetWorld();
	if (World)
	{
		for (int i = 0; i < 5; i++)
		{
			MapTile.Add(F2DTArray());
			int temp = FMath::RandRange(0, 1);
			int temp2 = temp ? 1 : -1;
			MapTile[i].Dir = temp2;
			
			for (int j = 0; j < 5; j++)
			{
				FVector Loc;
				Loc.X = BaseLocation.X - 1000 + (j * 500);
				Loc.Y = BaseLocation.Y - 1000 + (i * 500);
				Loc.Z = 4000.0f;
				
				MapTile[i].Add(FTileStruct(World->SpawnActor<ATile>(ATile::StaticClass(), Loc, FRotator::ZeroRotator), Loc.X, Loc.Y));
			}
		}
		
		for (int i = 0; i < 5; i++)
		{
			FVector Loc;
			Loc.X = BaseLocation.X + (MapTile[i].Dir * 1400.0f);
			Loc.Y = BaseLocation.Y - 1000 + (i * 500);
			Loc.Z = 4100.0f;
		
			FRotator Rot;
			Rot.Roll = 0.0f;
			Rot.Yaw = 0.0f;
			Rot.Pitch = MapTile[i].Dir * 150.0f;
		
			UObject* Object = StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("/Game/Assets/Mesh/hammer.hammer"));
			UBlueprint* BP = Cast<UBlueprint>(Object);
			TSubclassOf<class UObject> BP_Hammer = static_cast<UClass*>(BP->GeneratedClass);
			World->SpawnActor<AActor>(BP_Hammer, Loc, Rot);
			// AddHammer(World->SpawnActor<ATile>(ATile::StaticClass(), Loc, Rot), temp);
		}
		HammerTimeline->PlayFromStart();
	}

	// 기존 바닥 삭제
	for (TActorIterator<AStaticMeshActor> It(GetWorld()); It; ++It)
	{
		if (*It->GetName() == FString("SM_FloatingIsland_L_Island2"))
		{
			(*It)->Destroy();
		}
	}

}

void ANMMountainDragon::FireBallAttack_Implementation()
{
	bFireBallAttack = true;

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

void ANMMountainDragon::AddHammer_Implementation(AActor* Actor, int input)
{
	// Hammer.Add(FHammerStruct(Actor, input));
}

void ANMMountainDragon::LandInterp_Implementation(float Value)
{
	FVector NewLocation;
	NewLocation.X = FMath::Lerp(BaseLocation.X, OldLocation.X, Value);
	NewLocation.Y = FMath::Lerp(BaseLocation.Y, OldLocation.Y, Value);
	NewLocation.Z = FMath::Lerp(BaseLocation.Z, OldLocation.Z, Value);
	SetActorLocation(NewLocation);
}

void ANMMountainDragon::LandFinish_Implementation()
{
	if (HasAuthority())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		auto* NMAIController = Cast<ANMMountainDragonAIController>(GetController());

		if (bFly)
		{
			bFly = false;

			if (!FlyFireAttack)
			{
				NMAIController->GetBlackboardComponent()->SetValueAsBool(NMAIController->CanFireSpreadAttackKey, false);
				FlyFireAttack = true;
			}
			else
			{
				NMAIController->GetBlackboardComponent()->SetValueAsBool(NMAIController->CanFireBallAttackKey, false);
			}
		}
		else if (bGlideAttack)
		{
			GetCharacterMovement()->MaxFlySpeed = 600.0f;
			bGlideAttack = false;
			
			NMAIController->GetBlackboardComponent()->SetValueAsBool(NMAIController->CanGlideAttackKey, false);
		}
	}
}

void ANMMountainDragon::HammerInterp_Implementation(float Value)
{
	// for (int i = 0; i < 5; i++)
	// {
	// 	FRotator Rot;
	// 	Rot.Roll = 0.0f;
	// 	Rot.Yaw = 0.0f;
	// 	Rot.Pitch =  150 * Hammer[i].Dir * Value;
	//
	// 	Hammer[i].Actor->SetActorRotation(Rot);
	// }
}

void ANMMountainDragon::HammerFinish_Implementation()
{
	if (HasAuthority())
	{
		TileTimeline->PlayFromStart();
	}
}

void ANMMountainDragon::TileInterp_Implementation(float Value)
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

void ANMMountainDragon::TileFinish_Implementation()
{
	if (HasAuthority())
	{
		// for (int i = 0; i < 5; i++)
		// 	MapTile[i].Actor->Destroy();

		EndFly();
	}
}