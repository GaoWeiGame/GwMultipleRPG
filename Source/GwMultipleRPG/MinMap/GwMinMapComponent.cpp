// Fill out your copyright notice in the Description page of Project Settings.


#include "GwMinMapComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "GwMultipleRPG/Gameplay/GameModes/GwGameMode.h"
#include "GwMultipleRPG/Gameplay/Player/GwPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "W_GwMinMapPlayer.h"

UE_DISABLE_OPTIMIZATION

UGwMinMapComponent::UGwMinMapComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	  LowerRightPos(-2000, 2000),
	  UpperLeftPos(2000, -2000)
{
	SetIsReplicatedByDefault(true);
}

void UGwMinMapComponent::BeginPlay()
{
	Super::BeginPlay();
	AGameStateBase* GameState = GetGameState<AGameStateBase>();

	if (GameState)
	{
		AGwGameMode* GameMode = Cast<AGwGameMode>(GameState->AuthorityGameMode);
		if (GameMode)
		{			
			GameMode->OnSpawnPlayer.AddUObject(this, &ThisClass::PlayerLogin);
		}
	}
}


void UGwMinMapComponent::PlayerLogin(APawn* Pawn)
{
	AllPawns.AddUnique(Pawn);
}

TArray<APawn*> UGwMinMapComponent::GetAllPawn()
{
	return AllPawns;
}

void UGwMinMapComponent::RemovePawn(APawn* Pawn)
{
	AllPawns.Remove(Pawn);
	for (UW_GwMinMapPlayer* Item : GwMinMap->AllMinMapPlayer)
	{
		Item->RemoveFromParent();
	}
	GwMinMap->AllMinMapPlayer.Empty();
}

void UGwMinMapComponent::OnRep_AllPawn(const TArray<APawn*>& OldData)
{
	if (GwMinMap)
	{
		for (UW_GwMinMapPlayer* Item : GwMinMap->AllMinMapPlayer)
		{
			Item->RemoveFromParent();
		}
		GwMinMap->AllMinMapPlayer.Empty();
	}
	
	OnGwUpdateMinMapPlayer.Broadcast();
}

FVector2D UGwMinMapComponent::UpdateActorPosOnMap(AActor* InActor)
{
	return LocationToMapPos(InActor->GetActorLocation(), FVector2D(60));
}

FVector2D UGwMinMapComponent::LocationToMapPos(FVector InLocation, FVector2D InWidgetSize)
{
	FVector2D OutPut;
	OutPut.X = (ConvertLocationToMapLocation(InLocation).X / GetMapWidth() * GwMinMap->MapSizeX) - InWidgetSize.X / 2;
	OutPut.Y = (ConvertLocationToMapLocation(InLocation).Y / GetMapHeight() * GwMinMap->MapSizeY) - InWidgetSize.Y / 2;

	return OutPut;
}

FVector2D UGwMinMapComponent::ConvertLocationToMapLocation(FVector InLocation)
{
	return FVector2D(abs(InLocation.Y - UpperLeftPos.Y), abs(InLocation.X - UpperLeftPos.X));
}

void UGwMinMapComponent::SetGWMinMap(UW_GWMinMap* MinMap)
{
	GwMinMap = MinMap;
}

void UGwMinMapComponent::UpdateAllPlayerPos()
{
	for (APawn* Item : AllPawns)
	{
		bool bExistMinMap = false;
		if (GwMinMap)
		{
			for (UW_GwMinMapPlayer* MinMapPlayer : GwMinMap->AllMinMapPlayer)
			{
				if (MinMapPlayer->Pawn == Item)
				{
					MinMapPlayer->SetRenderTranslation(UpdateActorPosOnMap(Item));
					bExistMinMap = true;
					break;
				}
			}
			if (!bExistMinMap)
			{
				TSubclassOf<UW_GwMinMapPlayer> MinMapPlayerInfoClass = LoadClass<UW_GwMinMapPlayer>(GwMinMap, TEXT("/Script/UMGEditor.WidgetBlueprint'/RpgCore/UserInterface/MinMap/W_MinMapPlayer.W_MinMapPlayer_C'"));
				UW_GwMinMapPlayer* MinMapPlayer = CreateWidget<UW_GwMinMapPlayer>(GwMinMap, MinMapPlayerInfoClass);
				GwMinMap->Overlay->AddChild(MinMapPlayer);
				MinMapPlayer->Pawn = Item;
				GwMinMap->AllMinMapPlayer.AddUnique(MinMapPlayer);
				MinMapPlayer->SetRenderTranslation(UpdateActorPosOnMap(Item));
			}
		}
	}
}

float UGwMinMapComponent::GetMapWidth() const
{
	return abs(LowerRightPos.Y - UpperLeftPos.Y);
}

float UGwMinMapComponent::GetMapHeight() const
{
	return abs(LowerRightPos.X - UpperLeftPos.X);
}


void UGwMinMapComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, AllPawns, COND_None, REPNOTIFY_Always);
}


UE_ENABLE_OPTIMIZATION
