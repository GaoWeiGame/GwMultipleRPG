// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogActionRPG, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogGwAbilitySystem, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogGwExperience, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogGw, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogGwTeams, Log, All);

GWMULTIPLERPG_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
