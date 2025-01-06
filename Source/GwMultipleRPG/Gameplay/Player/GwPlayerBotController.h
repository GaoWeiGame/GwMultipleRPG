// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularAIController.h"
#include "GwMultipleRPG/Teams/GwTeamAgentInterface.h"
#include "GwPlayerBotController.generated.h"

UCLASS()
class GWMULTIPLERPG_API AGwPlayerBotController : public AModularAIController, public IGwTeamAgentInterface
{
	GENERATED_BODY()

public:
	AGwPlayerBotController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnGwTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;


	virtual void OnUnPossess() override;


protected:
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;

	virtual void OnPlayerStateChanged();

private:

	void BroadcastOnPlayerStateChanged();

	UFUNCTION()
	void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);
	
	UPROPERTY()
	FOnGwTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;
};
