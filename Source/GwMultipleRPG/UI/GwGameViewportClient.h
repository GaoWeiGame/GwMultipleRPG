// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameViewportClient.h"
#include "GwGameViewportClient.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GWMULTIPLERPG_API UGwGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()
public:
	UGwGameViewportClient();

	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;
};
