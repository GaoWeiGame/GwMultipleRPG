// Fill out your copyright notice in the Description page of Project Settings.


#include "GwChatSystemMenuInterface.h"


// Add default functionality here for any IGwChatSystemMenuInterface functions that are not pure virtual.
bool IGwChatSystemMenuInterface::AddNewMsg(EChatMessageType InMessageType, APlayerState* InPS, FText InMessage)
{
	return true;
}
