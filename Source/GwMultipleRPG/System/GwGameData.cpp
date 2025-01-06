// Copyright Epic Games, Inc. All Rights Reserved.

#include "GwGameData.h"
#include "GwMultipleRPG/GwAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GwGameData)

UGwGameData::UGwGameData()
{
}

const UGwGameData& UGwGameData::UGwGameData::Get()
{
	return UGwAssetManager::Get().GetGameData();
}
