// Fill out your copyright notice in the Description page of Project Settings.


#include "UGwChatSystem_GameStateComponent.h"
#include "GameFramework/PlayerState.h"
#include "GwMultipleRPG/Teams/GwTeamSubsystem.h"


UGwChatSystem_GameStateComponent::UGwChatSystem_GameStateComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

TArray<APlayerState*> UGwChatSystem_GameStateComponent::GetPlayersByMsgType(EChatMessageType InMessageType, APlayerState* InPS)
{
	TArray<APlayerState*> OutPut;

	AGameStateBase* GameState = GetGameState<AGameStateBase>();
	switch (InMessageType)
	{
	case EChatMessageType::All:
		OutPut = GameState->PlayerArray;
		break;
	case EChatMessageType::Team:
		{
			UGwTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UGwTeamSubsystem>();
			for (APlayerState* PS : GameState->PlayerArray)
			{
				EGwTeamComparison TeamComparison = TeamSubsystem->CompareTeams(InPS, PS);
				if (TeamComparison == EGwTeamComparison::OnSameTeam)
				{
					OutPut.Add(PS);
				}
			}
		}
		break;
	default:
		break;
	}
	return OutPut;
}
