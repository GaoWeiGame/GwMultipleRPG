// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "GwMultipleRPG/System/GameplayTagStack.h"
#include "GwTeamInfoBase.generated.h"

class UGwTeamCreationComponent;
class UGwTeamSubsystem;
struct FGwTeamTrackingInfo;

UCLASS()
class GWMULTIPLERPG_API AGwTeamInfoBase : public AInfo
{
	GENERATED_BODY()
	friend UGwTeamCreationComponent;

public:
	// Sets default values for this actor's properties
	AGwTeamInfoBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	int32 GetTeamId() const { return TeamId; }

	UPROPERTY(Replicated)
	FGameplayTagStackContainer TeamTags;
	
protected:
	virtual void RegisterWithTeamSubsystem(UGwTeamSubsystem* Subsystem);

	void TryRegisterWithTeamSubsystem();

private:
	void SetTeamId(int32 NewTeamId);

	UFUNCTION()
	void OnRep_TeamId();

	UPROPERTY(ReplicatedUsing=OnRep_TeamId)
	int32 TeamId;
};
