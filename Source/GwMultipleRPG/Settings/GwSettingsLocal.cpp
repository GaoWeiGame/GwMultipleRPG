// Fill out your copyright notice in the Description page of Project Settings.


#include "GwSettingsLocal.h"

#include "Widgets/Layout/SSafeZone.h"

UGwSettingsLocal* UGwSettingsLocal::Get()
{
	return GEngine ? CastChecked<UGwSettingsLocal>(GEngine->GetGameUserSettings()) : nullptr;
}

void UGwSettingsLocal::ApplySafeZoneScale()
{
	SSafeZone::SetGlobalSafeZoneScale(GetSafeZone());
}
