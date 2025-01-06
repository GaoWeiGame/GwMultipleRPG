// Fill out your copyright notice in the Description page of Project Settings.


#include "W_GwChat.h"

bool UW_GwChat::AddNewMsg(EChatMessageType InMessageType, APlayerState* InPS, FText InMessage)
{
	BP_AddNewMsg(InMessageType, InPS, InMessage);
	return true;
}
