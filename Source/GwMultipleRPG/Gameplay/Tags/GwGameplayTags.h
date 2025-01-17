﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

namespace GwGameplayTags
{
	GWMULTIPLERPG_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	// Declare all of the custom native tags that Lyra will use
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cooldown);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cost);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsBlocked);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsMissing);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Networking);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);

	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Behavior_SurvivesDeath);

	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AutoRun);

	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Reset);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_RequestReset);

	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Heal);

	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_GodMode);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_UnlimitedHealth);

	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Crouching);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_AutoRunning);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dying);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dead);

	// These are mappings from MovementMode enums to GameplayTags associated with those enums (below)
	GWMULTIPLERPG_API	extern const TMap<uint8, FGameplayTag> MovementModeTagMap;
	GWMULTIPLERPG_API	extern const TMap<uint8, FGameplayTag> CustomMovementModeTagMap;

	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_NavWalking);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Falling);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Swimming);
	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Flying);

	GWMULTIPLERPG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Custom);
};
