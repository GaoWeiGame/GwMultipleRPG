// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/CancellableAsyncAction.h"
#include "AsyncAction_ObserveTeam.generated.h"

class IGwTeamAgentInterface;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTeamObservedAsyncDelegate, bool, bTeamSet, int32, TeamId);


UCLASS()
class GWMULTIPLERPG_API UAsyncAction_ObserveTeam : public UCancellableAsyncAction
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", Keywords="Watch"))
	static UAsyncAction_ObserveTeam* ObserveTeam(UObject* TeamAgent);

	virtual void Activate() override;
	virtual void SetReadyToDestroy() override;

public:
	// Called when the team is set or changed
	UPROPERTY(BlueprintAssignable)
	FTeamObservedAsyncDelegate OnTeamChanged;

private:
	static UAsyncAction_ObserveTeam* InternalObserveTeamChanges(TScriptInterface<IGwTeamAgentInterface> TeamActor);

private:
	UFUNCTION()
	void OnWatchedAgentChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

	TWeakInterfacePtr<IGwTeamAgentInterface> TeamInterfacePtr;
};
