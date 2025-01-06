// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwGameplayAbility.h"
#include "GwGameplayAbilityMinMap.generated.h"

class UGwTaggedWidget;
/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UGwGameplayAbilityMinMap : public UGwGameplayAbility
{
	GENERATED_BODY()

public:
	UGwGameplayAbilityMinMap(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void UpdatePlayerPositionOnMap();

	FVector2D LocationToMapPosition(const FVector& Location, const FVector2D& WidgetSize) const;

	FVector2D ConvertLocationToMapLocation(const FVector2D& Location) const;

	FORCEINLINE float GetMapWidth() const { return abs((UpperLeftLocation.Y - DownRightLocation.Y)); }
	FORCEINLINE float GetMapHeight() const { return abs((UpperLeftLocation.X - DownRightLocation.X)); }

private:
	UPROPERTY()
	TObjectPtr<UGwTaggedWidget> W_MinMap;

	UPROPERTY()
	FVector2D UpperLeftLocation;

	UPROPERTY()
	FVector2D DownRightLocation;

	UPROPERTY()
	int32 MapSizeX;

	UPROPERTY()
	int32 MapSizeY;
};
