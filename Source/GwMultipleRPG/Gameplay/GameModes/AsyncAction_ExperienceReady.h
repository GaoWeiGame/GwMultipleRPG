﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_ExperienceReady.generated.h"

class UGwExperienceDefinition;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExperienceReadyAsyncDelegate);

UCLASS()
class GWMULTIPLERPG_API UAsyncAction_ExperienceReady : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext = "WorldContextObject", BlueprintInternalUseOnly="true"))
	static UAsyncAction_ExperienceReady* WaitForExperienceReady(UObject* WorldContextObject);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FExperienceReadyAsyncDelegate OnReady;
private:
	TWeakObjectPtr<UWorld> WorldPtr;

	void Step1_HandleGameStateSet(AGameStateBase* GameState);
	void Step2_ListenToExperienceLoading(AGameStateBase* GameState);
	void Step3_HandleExperienceLoaded(const UGwExperienceDefinition* CurrentExperience);
	void Step4_BroadcastReady();
};


