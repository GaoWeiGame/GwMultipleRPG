// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwChatSystemMenuInterface.h"
#include "GwMultipleRPG/UI/GwTaggedWidget.h"
#include "W_GwChat.generated.h"

/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UW_GwChat : public UGwTaggedWidget, public IGwChatSystemMenuInterface
{
	GENERATED_BODY()

public:
	virtual bool AddNewMsg(EChatMessageType InMessageType, APlayerState* InPS, FText InMessage) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_AddNewMsg(EChatMessageType InMessageType, APlayerState* InPS, const FText& InMessage);
};
