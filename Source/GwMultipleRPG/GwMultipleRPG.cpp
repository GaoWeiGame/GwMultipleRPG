// Copyright Epic Games, Inc. All Rights Reserved.

#include "GwMultipleRPG.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, GwMultipleRPG, "GwMultipleRPG" );

DEFINE_LOG_CATEGORY(LogActionRPG);
DEFINE_LOG_CATEGORY(LogGwAbilitySystem);
DEFINE_LOG_CATEGORY(LogGwExperience);
DEFINE_LOG_CATEGORY(LogGw);
DEFINE_LOG_CATEGORY(LogGwTeams);

FString GetClientServerContextString(UObject* ContextObject)
{
	ENetRole Role = ROLE_None;

	if (AActor* Actor = Cast<AActor>(ContextObject))
	{
		Role = Actor->GetLocalRole();
	}
	else if (UActorComponent* Component = Cast<UActorComponent>(ContextObject))
	{
		Role = Component->GetOwnerRole();
	}

	if (Role != ROLE_None)
	{
		return (Role == ROLE_Authority) ? TEXT("Server") : TEXT("Client");
	}
	else
	{
#if WITH_EDITOR
		if (GIsEditor)
		{
			extern ENGINE_API FString GPlayInEditorContextString;
			return GPlayInEditorContextString;
		}
#endif
	}

	return TEXT("[]");
}
