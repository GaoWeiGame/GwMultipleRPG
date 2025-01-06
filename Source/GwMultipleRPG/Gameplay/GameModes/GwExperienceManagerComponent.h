// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeaturePluginOperationResult.h"
#include "Components/GameStateComponent.h"
#include "GwExperienceManagerComponent.generated.h"


class UGwExperienceDefinition;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGwExperienceLoaded, const UGwExperienceDefinition* /*Experience*/);


enum class EGwExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	LoadingChaosTestingDelay,
	ExecutingActions,
	Loaded,
	Deactivating
};

UCLASS()
class GWMULTIPLERPG_API UGwExperienceManagerComponent final: public UGameStateComponent
{
	GENERATED_BODY()
public:
	UGwExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
	void SetCurrentExperience(FPrimaryAssetId ExperienceId);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool IsExperienceLoaded() const;

	void CallOrRegister_OnExperienceLoaded(FOnGwExperienceLoaded::FDelegate&& Delegate);

	void CallOrRegister_OnExperienceLoaded_HighPriority(FOnGwExperienceLoaded::FDelegate&& Delegate);

	void CallOrRegister_OnExperienceLoaded_LowPriority(FOnGwExperienceLoaded::FDelegate&& Delegate);

	const UGwExperienceDefinition* GetCurrentExperienceChecked() const;

private:
	void StartExperienceLoad();
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentExperience)
	TObjectPtr<const UGwExperienceDefinition> CurrentExperience;

	UFUNCTION()
	void OnRep_CurrentExperience();

	EGwExperienceLoadState LoadState = EGwExperienceLoadState::Unloaded;
	void OnExperienceLoadComplete();

	TArray<FString> GameFeaturePluginURLs;
	int32 NumGameFeaturePluginsLoading = 0;

	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();

	FOnGwExperienceLoaded OnExperienceLoaded;

	FOnGwExperienceLoaded OnExperienceLoaded_HighPriority;

	FOnGwExperienceLoaded OnExperienceLoaded_LowPriority;

	int32 NumObservedPausers = 0;
	int32 NumExpectedPausers = 0;

	void OnActionDeactivationCompleted();
	void OnAllActionsDeactivated();
};
