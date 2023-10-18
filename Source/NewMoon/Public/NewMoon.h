// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

UENUM(BlueprintType)
enum class ECharacterMotion : uint8
{
	ECM_Stand UMETA(DisplayName = "Stand"),
	ECM_Crouch UMETA(DisplayName = "Crouch"),
	ECM_Prone UMETA(DisplayName = "Prone"),

	ECM_MAX UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Idle UMETA(DisplayName = "Idle"),
	ECS_Battle UMETA(DisplayName = "Battle"),
	ECS_Dead UMETA(DisplayName = "Dead"),

	ECS_MAX UMETA(DisplayName = "DefaultMax")
};

DECLARE_LOG_CATEGORY_EXTERN(NewMoon, Log, All);
#define NMLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define NMLOG_S(Verbosity) UE_LOG(NewMoon, Verbosity, TEXT("%s"), *NMLOG_CALLINFO)
#define NMLOG(Verbosity, Format, ...) UE_LOG(NewMoon, Verbosity, TEXT("%s %s"), *NMLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define NMCHECK(Expr, ...) {if(!(Expr)) {NMLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}


#define ECC_NMEnemy ECC_GameTraceChannel4
#define ECC_EnemyAttack ECC_GameTraceChannel2
#define ECC_NMCharacter ECC_GameTraceChannel1
#define ECC_PlayerAttack ECC_GameTraceChannel3

