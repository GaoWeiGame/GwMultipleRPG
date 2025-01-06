// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameInstance.h"
#include "Engine/GameInstance.h"
#include "GwGameInstance.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class GWMULTIPLERPG_API UGwGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()
public:

	UGwGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void Init() override;
	virtual void Shutdown() override;
	
	void OnPreClientTravelToSession(FString& URL);

};
