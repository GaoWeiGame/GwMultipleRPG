// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwTeamInfoBase.h"
#include "GwTeamPublicInfo.generated.h"

class UGwTeamCreationComponent;
class UGwTeamDisplayAsset;

UCLASS()
class GWMULTIPLERPG_API AGwTeamPublicInfo : public AGwTeamInfoBase
{
	GENERATED_BODY()
	friend UGwTeamCreationComponent;

public:
	// Sets default values for this actor's properties
	AGwTeamPublicInfo();

	UGwTeamDisplayAsset* GetTeamDisplayAsset() const { return TeamDisplayAsset; }

private:

	UFUNCTION()
	void OnRep_TeamDisplayAsset();

	void SetTeamDisplayAsset(TObjectPtr<UGwTeamDisplayAsset> NewDisplayAsset);
	
	UPROPERTY(ReplicatedUsing=OnRep_TeamDisplayAsset)
	TObjectPtr<UGwTeamDisplayAsset> TeamDisplayAsset;
};
