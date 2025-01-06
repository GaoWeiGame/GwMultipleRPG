// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonLocalPlayer.h"
#include "GwMultipleRPG/Teams/GwTeamAgentInterface.h"
#include "GwLocalPlayer.generated.h"

class UGwSettingsLocal;
class UGwSettingsShared;

UCLASS()
class GWMULTIPLERPG_API UGwLocalPlayer : public UCommonLocalPlayer, public IGwTeamAgentInterface
{
	GENERATED_BODY()

public:

	UGwLocalPlayer();
	
	UFUNCTION()
	UGwSettingsShared* GetSharedSettings() const;


	virtual void PostInitProperties() override;

	virtual void SwitchController(class APlayerController* PC) override;

	virtual bool SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld) override;
	virtual void InitOnlineSession() override;
	
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnGwTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;

	UFUNCTION()
	UGwSettingsLocal* GetLocalSettings() const;
	
protected:
	void OnPlayerControllerChanged(APlayerController* NewController);

	UFUNCTION()
	void OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);
	
private:
	UPROPERTY(Transient)
	mutable TObjectPtr<UGwSettingsShared> SharedSettings;

	UPROPERTY()
	FOnGwTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> LastBoundPC;
};
