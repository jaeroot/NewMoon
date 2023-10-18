// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"

#include "Character/NMCharacter.h"
#include "Components/CombatComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);
	
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECC_NMCharacter, ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECC_NMEnemy, ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECC_NMCharacter, ECollisionResponse::ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	}
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ANMCharacter* NMCharacter = Cast<ANMCharacter>(OtherActor))
	{
		NMCharacter->SetOverlappingActor(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ANMCharacter* NMCharacter = Cast<ANMCharacter>(OtherActor))
	{
		NMCharacter->SetOverlappingActor(nullptr);
	}
}

FItemData AItem::Interact()
{
	
	return FItemData(ItemID, Name, Thumbnail);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::UseItem()
{
	if (HasAuthority())
	{
		if (ANMCharacter* NMCharacter = Cast<ANMCharacter>(GetOwner()))
		{
			NMCharacter->Combat->TakeDamage(-25.0f);
		}
	}
	else
	{
		ServerUseItem();
	}
}

void AItem::ServerUseItem_Implementation()
{
	if (ANMCharacter* NMCharacter = Cast<ANMCharacter>(GetOwner()))
	{
		NMCharacter->Combat->TakeDamage(-25.0f);
	}
}
