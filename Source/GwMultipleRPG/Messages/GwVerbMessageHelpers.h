// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GwVerbMessageHelpers.generated.h"

/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UGwVerbMessageHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Gw")
	static APlayerState* GetPlayerStateFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Gw")
	static APlayerController* GetPlayerControllerFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Gw")
	static FGameplayCueParameters VerbMessageToCueParameters(const FGwVerbMessage& Message);

	UFUNCTION(BlueprintCallable, Category = "Gw")
	static FGwVerbMessage CueParametersToVerbMessage(const FGameplayCueParameters& Params);
};
