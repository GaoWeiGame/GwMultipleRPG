// Fill out your copyright notice in the Description page of Project Settings.


#include "GwGameplayAbilityMinMap.h"


UGwGameplayAbilityMinMap::UGwGameplayAbilityMinMap(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	  UpperLeftLocation(-4000, -4000),
	  DownRightLocation(4000, 4000)
{
	MapSizeX = 500;
	MapSizeY = 500;
}

void UGwGameplayAbilityMinMap::UpdatePlayerPositionOnMap()
{
	return;;
}

FVector2D UGwGameplayAbilityMinMap::LocationToMapPosition(const FVector& Location, const FVector2D& WidgetSize) const
{
	FVector2D OutPut;
	const FVector2D TmpPos = ConvertLocationToMapLocation(FVector2D(Location));
	OutPut.X = TmpPos.X / GetMapWidth() * MapSizeX - WidgetSize.X / 2;
	OutPut.Y = TmpPos.Y / GetMapHeight() * MapSizeY - WidgetSize.Y / 2;
	return OutPut;
}

FVector2D UGwGameplayAbilityMinMap::ConvertLocationToMapLocation(const FVector2D& Location) const
{
	return FVector2D(abs(Location.X - UpperLeftLocation.X), abs(Location.Y - UpperLeftLocation.Y));
}
