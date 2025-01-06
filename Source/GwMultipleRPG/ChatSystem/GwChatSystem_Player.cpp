// Fill out your copyright notice in the Description page of Project Settings.


#include "GwChatSystem_Player.h"

#include "UGwChatSystem_GameStateComponent.h"
#include "W_GwChat.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

UE_DISABLE_OPTIMIZATION

UGwChatSystem_Player::UGwChatSystem_Player(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UGwChatSystem_Player::CommitChatMsg(EChatMessageType InMessageType, APlayerState* InPS, FText InMessage)
{
	SendMsg_Server(InMessageType, InPS, InMessage);
}

void UGwChatSystem_Player::SendMsg_Client_Implementation(EChatMessageType InMessageType, APlayerState* InPS, const FText& InMessage)
{
	ChatWidget->AddNewMsg(InMessageType, InPS, InMessage);
}

void UGwChatSystem_Player::SendMsg_Server_Implementation(EChatMessageType InMessageType, APlayerState* InPS, const FText& InMessage)
{
	UGwChatSystem_GameStateComponent* ChatGSC = UGameplayStatics::GetGameState(GetWorld())->GetComponentByClass<UGwChatSystem_GameStateComponent>();
	IGwChatSystemInterface* IChatSystem = Cast<IGwChatSystemInterface>(ChatGSC);
	TArray<APlayerState*> AllPlayerState = IChatSystem->GetPlayersByMsgType(InMessageType, InPS);
	for (APlayerState* PS : AllPlayerState)
	{
		
		UGwChatSystem_Player* ChatSystem_Player = PS->GetOwningController()->GetComponentByClass<UGwChatSystem_Player>();
		if (ChatSystem_Player)
		{
			ChatSystem_Player->SendMsg_Client(InMessageType, InPS, InMessage);
		}
	}
}

UE_ENABLE_OPTIMIZATION
