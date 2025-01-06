// Fill out your copyright notice in the Description page of Project Settings.


#include "GwWorldSettings.h"

#include "EngineUtils.h"
#include "GwExperienceDefinition.h"
#include "Engine/AssetManager.h"
#include "GameFramework/PlayerStart.h"
#include "GwMultipleRPG/GwAssetManager.h"
#include "GwMultipleRPG/GwMultipleRPG.h"
#include "Misc/UObjectToken.h"
UE_DISABLE_OPTIMIZATION

AGwWorldSettings::AGwWorldSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FPrimaryAssetId AGwWorldSettings::GetDefaultGameplayExperience() const
{
	FPrimaryAssetId Result;
	if (!DefaultGameplayExperience.IsNull())
	{
		UGwAssetManager& AssetManager = UGwAssetManager::Get();
		// FSoftObjectPath ObjectPath = DefaultGameplayExperience.ToSoftObjectPath();
		FSoftObjectPath ObjectPath = DefaultGameplayExperience.GetUniqueID();
		Result = AssetManager.GetPrimaryAssetIdForPath(ObjectPath);
		
		// Result = UAssetManager::Get().GetPrimaryAssetIdForPath(DefaultGameplayExperience.ToSoftObjectPath());
		if (!Result.IsValid())
		{
			UE_LOG(LogGwExperience, Error, TEXT("%s.DefaultGameplayExperience is %s but that failed to resolve into an asset ID (you might need to add a path to the Asset Rules in your game feature plugin or project settings"),
			       *GetPathNameSafe(this), *DefaultGameplayExperience.ToString());
		}
	}
	return Result;
}

#if WITH_EDITOR

void AGwWorldSettings::CheckForErrors()
{
	Super::CheckForErrors();

	FMessageLog MapCheck("MapCheck");

	for (TActorIterator<APlayerStart> PlayerStartIt(GetWorld()); PlayerStartIt; ++PlayerStartIt)
	{
		APlayerStart* PlayerStart = *PlayerStartIt;
		if (IsValid(PlayerStart) && PlayerStart->GetClass() == APlayerStart::StaticClass())
		{
			MapCheck.Warning()
			        ->AddToken(FUObjectToken::Create(PlayerStart))
			        ->AddToken(FTextToken::Create(FText::FromString("is a normal APlayerStart, replace with ALyraPlayerStart.")));
		}
	}
}
#endif

UE_ENABLE_OPTIMIZATION
