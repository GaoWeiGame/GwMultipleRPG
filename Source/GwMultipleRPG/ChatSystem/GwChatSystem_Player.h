// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwChatSystemInterface.h"
#include "Components/PlayerStateComponent.h"
#include "GwChatSystem_Player.generated.h"


class UW_GwChat;

UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class GWMULTIPLERPG_API UGwChatSystem_Player : public UActorComponent, public IGwChatSystemInterface
{
	GENERATED_BODY()

public:
	UGwChatSystem_Player(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	virtual void CommitChatMsg(EChatMessageType InMessageType, APlayerState* InPS, FText InMessage) override;

	UFUNCTION(Server,Reliable)
	void SendMsg_Server(EChatMessageType InMessageType, APlayerState* InPS, const FText& InMessage);

	UFUNCTION(Client,Reliable)
	void SendMsg_Client(EChatMessageType InMessageType, APlayerState* InPS, const FText& InMessage);

	UPROPERTY(BlueprintReadWrite)
	UW_GwChat* ChatWidget;
};
