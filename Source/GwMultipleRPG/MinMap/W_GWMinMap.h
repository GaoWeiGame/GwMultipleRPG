// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "W_GwMinMapPlayer.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Overlay.h"
#include "GwMultipleRPG/UI/GwTaggedWidget.h"
#include "W_GWMinMap.generated.h"

class UW_GwMinMapPlayer;

USTRUCT()
struct FMinMapPlayerInfo
{
	GENERATED_BODY()

	FMinMapPlayerInfo(): Pawn(nullptr), MinMapPlayer(nullptr)
	{
	};

	FMinMapPlayerInfo(APawn* InPawn)
	{
		Pawn = InPawn;
		MinMapPlayer = nullptr;
	}

	FMinMapPlayerInfo(APawn* InPawn, UW_GwMinMapPlayer* InMinMapPlayer)
	{
		Pawn = InPawn;
		MinMapPlayer = InMinMapPlayer;
	}

	UPROPERTY()
	APawn* Pawn;

	UPROPERTY(NotReplicated)
	UW_GwMinMapPlayer* MinMapPlayer;

	bool operator==(const FMinMapPlayerInfo& A) const
	{
		return this->Pawn == A.Pawn;
	}
};

UCLASS()
class GWMULTIPLERPG_API UW_GWMinMap : public UGwTaggedWidget
{
	GENERATED_BODY()

public:
	UW_GWMinMap(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(meta=(Bindwidget))
	UOverlay* Overlay;

	UPROPERTY()
	TArray<UW_GwMinMapPlayer*> AllMinMapPlayer;

	int32 MapSizeX;
	int32 MapSizeY;
};
