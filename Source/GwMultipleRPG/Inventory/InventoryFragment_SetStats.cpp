﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryFragment_SetStats.h"

#include "GameplayTagContainer.h"
#include "GwInventoryItemInstance.h"

void UInventoryFragment_SetStats::OnInstanceCreated(UGwInventoryItemInstance* Instance) const
{
	for (const auto& KVP : InitialItemStats)
	{
		Instance->AddStatTagStack(KVP.Key, KVP.Value);
	}
}

int32 UInventoryFragment_SetStats::GetItemStatByTag(FGameplayTag Tag) const
{
	if (const int32* StatPtr = InitialItemStats.Find(Tag))
	{
		return *StatPtr;
	}

	return 0;
}
