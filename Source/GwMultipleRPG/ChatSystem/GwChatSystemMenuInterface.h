// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GwChatSystemMenuInterface.generated.h"

enum EChatMessageType : uint8;
// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UGwChatSystemMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GWMULTIPLERPG_API IGwChatSystemMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool AddNewMsg(EChatMessageType InMessageType, APlayerState* InPS, FText InMessage);
};
