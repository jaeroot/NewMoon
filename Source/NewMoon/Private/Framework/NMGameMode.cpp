// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/NMGameMode.h"

#include "Character/NMCharacter.h"

ANMGameMode::ANMGameMode()
{
	DefaultPawnClass = ANMCharacter::StaticClass();
}
