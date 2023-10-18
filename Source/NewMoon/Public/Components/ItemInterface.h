// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NewMoon.h"
#include "Engine/DataTable.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()

};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	FItemData(): ID(0), Name(""), Thumbnail(nullptr) {}

	FItemData(int NewID, FString NewName, UTexture2D* NewThumbnail) : ID(NewID), Name(NewName), Thumbnail(NewThumbnail) {}
};

class NEWMOON_API IItemInterface
{
	GENERATED_BODY()

public:
	virtual FItemData Interact() = 0;
};
