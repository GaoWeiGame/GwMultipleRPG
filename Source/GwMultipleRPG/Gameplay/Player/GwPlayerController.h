// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "GwMultipleRPG/Teams/GwTeamAgentInterface.h"
#include "GwPlayerController.generated.h"

class AGwPlayerState;
class APawn;

/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API AGwPlayerController : public ACommonPlayerController, public IGwTeamAgentInterface
{
	GENERATED_BODY()

public:
	AGwPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	
	UFUNCTION(BlueprintCallable, Category = "Gw|Character")
	void SetIsAutoRunning(const bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Gw|Character")
	bool GetIsAutoRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Gw|PlayerController")
	UGwAbilitySystemComponent* GetGwAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Gw|PlayerController")
	AGwPlayerState* GetGwPlayerState() const;

	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;


	//~IGwTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnGwTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of IGwTeamAgentInterface interface
	
protected:
	void OnStartAutoRun();
	void OnEndAutoRun();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnStartAutoRun"))
	void K2_OnStartAutoRun();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnEndAutoRun"))
	void K2_OnEndAutoRun();


	virtual void OnPlayerStateChanged();
	
private:
	void BroadcastOnPlayerStateChanged();

	UFUNCTION()
	void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);
	
	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;

	UPROPERTY()
	FOnGwTeamIndexChangedDelegate OnTeamChangedDelegate;
};
