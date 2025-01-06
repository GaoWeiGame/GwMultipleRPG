// Fill out your copyright notice in the Description page of Project Settings.


#include "GwChatSystemInterface.h"


// Add default functionality here for any IGwChatSystemInterface functions that are not pure virtual.
void IGwChatSystemInterface::CommitChatMsg(EChatMessageType InMessageType, APlayerState* InPS, FText InMessage)
{
}

TArray<APlayerState*> IGwChatSystemInterface::GetPlayersByMsgType(EChatMessageType InMessageType, APlayerState* InPS)
{
	return TArray<APlayerState*>();
}


