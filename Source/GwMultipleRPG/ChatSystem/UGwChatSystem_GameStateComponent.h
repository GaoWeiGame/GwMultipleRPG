// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GwChatSystemInterface.h"

#include "UGwChatSystem_GameStateComponent.generated.h"


enum EChatMessageType : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GWMULTIPLERPG_API UGwChatSystem_GameStateComponent : public UGameStateComponent,public IGwChatSystemInterface
{
	GENERATED_BODY()

public:

	UGwChatSystem_GameStateComponent(const FObjectInitializer& ObjectInitializer);

	virtual TArray<APlayerState*> GetPlayersByMsgType(EChatMessageType InMessageType, APlayerState* InPS) override;

};
