// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"

#include "AI/MountainDragon/NMMountainDragon.h"
#include "Character/NMCharacter.h"
#include "Components/CombatComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_NMCharacter, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECC_NMEnemy, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECC_NMCharacter, ECollisionResponse::ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponState);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ANMCharacter* NMCharacter = Cast<ANMCharacter>(OtherActor))
	{
		NMCharacter->SetOverlappingWeapon(this);
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ANMCharacter* NMCharacter = Cast<ANMCharacter>(OtherActor))
	{
		NMCharacter->SetOverlappingWeapon(nullptr);
	}
}

void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;

	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AWeapon::ServerAttack_Implementation()
{
	const USkeletalMeshSocket* MuzzleFlashSocket = WeaponMesh->GetSocketByName(FName("MuzzleFlash"));
	if (MuzzleFlashSocket)
	{
		FHitResult HitResult;
		FVector StartTrace = MuzzleFlashSocket->GetSocketLocation(WeaponMesh);
		FVector EndTrace = (Cast<ANMCharacter>(GetOwner())->Camera->GetForwardVector() * 3000.0f) + Cast<ANMCharacter>(GetOwner())->Camera->GetComponentLocation();
		bool bResult = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartTrace,
			EndTrace,
			ECC_PlayerAttack
		);

		if (bResult)
		{
			if (HitResult.GetActor()->IsA(ANMMountainDragon::StaticClass()))
			{
				Cast<ANMMountainDragon>(HitResult.GetActor())->Combat->TakeDamage(50.0f);
			}
		}
		
#if ENABLE_DRAW_DEBUG
		DrawDebugLine(
			GetWorld(),
			StartTrace,
			EndTrace,
			bResult ? FColor::Green : FColor::Red,
			false,
			5.0f
		);
#endif
	}

}

void AWeapon::OnRep_WeaponState()
{
	
}
