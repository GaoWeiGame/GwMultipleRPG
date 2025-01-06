// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayAbilitySpecHandle.h"
#include "GwGamePhaseSubsystem.generated.h"

struct FGameplayAbilitySpecHandle;
class UGwGamePhaseAbility;
DECLARE_DYNAMIC_DELEGATE_OneParam(FGwGamePhaseDynamicDelegate, const UGwGamePhaseAbility*, Phase);
DECLARE_DELEGATE_OneParam(FGwGamePhaseDelegate, const UGwGamePhaseAbility* Phase);

DECLARE_DELEGATE_OneParam(FGwGamePhaseTagDelegate, const FGameplayTag& PhaseTag);

UENUM(BlueprintType)
enum class EPhaseTagMatchType : uint8
{
	ExactMatch,

	PartialMatch
};

UCLASS()
class GWMULTIPLERPG_API UGwGamePhaseSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	friend class UGwGamePhaseAbility;
public:
	UGwGamePhaseSubsystem();

	virtual void PostInitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	void StartPhase(TSubclassOf<UGwGamePhaseAbility> PhaseAbility, FGwGamePhaseDelegate PhaseEndedCallback = FGwGamePhaseDelegate());

protected:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Game Phase", meta = (DisplayName="Start Phase", AutoCreateRefTerm = "PhaseEnded"))
	void K2_StartPhase(TSubclassOf<UGwGamePhaseAbility> PhaseAbility, const FGwGamePhaseDynamicDelegate& PhaseEndedDelegate);

	void OnBeginPhase(const UGwGamePhaseAbility* PhaseAbility, const FGameplayAbilitySpecHandle PhaseAbilityHandle);
	void OnEndPhase(const UGwGamePhaseAbility* PhaseAbility, const FGameplayAbilitySpecHandle PhaseAbilityHandle);
	
private:
	struct FGwGamePhaseEntry
	{
	public:
		FGameplayTag PhaseTag;
		FGwGamePhaseDelegate PhaseEndedCallback;
	};

	TMap<FGameplayAbilitySpecHandle, FGwGamePhaseEntry> ActivePhaseMap;

	struct FPhaseObserver
	{
	public:
		bool IsMatch(const FGameplayTag& ComparePhaseTag) const;
	
		FGameplayTag PhaseTag;
		EPhaseTagMatchType MatchType = EPhaseTagMatchType::ExactMatch;
		FGwGamePhaseTagDelegate PhaseCallback;
	};
	TArray<FPhaseObserver> PhaseStartObservers;
	TArray<FPhaseObserver> PhaseEndObservers;
};
