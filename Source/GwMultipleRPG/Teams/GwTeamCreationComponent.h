// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GwTeamCreationComponent.generated.h"


class UGwExperienceDefinition;
class AGwTeamPrivateInfo;
class AGwTeamPublicInfo;
class UGwTeamDisplayAsset;
class AGwPlayerState;

UCLASS(Blueprintable)
class GWMULTIPLERPG_API UGwTeamCreationComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UGwTeamCreationComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	
	virtual void BeginPlay() override;

protected:

	virtual void ServerCreateTeams();
	virtual void ServerAssignPlayersToTeams();

	virtual void ServerChooseTeamForPlayer(AGwPlayerState* PS);

	UPROPERTY(EditDefaultsOnly, Category = Teams)
	TMap<uint8, TObjectPtr<UGwTeamDisplayAsset>> TeamsToCreate;

	UPROPERTY(EditDefaultsOnly, Category=Teams)
	TSubclassOf<AGwTeamPublicInfo> PublicTeamInfoClass;

	UPROPERTY(EditDefaultsOnly, Category=Teams)
	TSubclassOf<AGwTeamPrivateInfo> PrivateTeamInfoClass;

private:
	void OnExperienceLoaded(const UGwExperienceDefinition* Experience);

	void OnPlayerInitialized(AGameModeBase* GameMode, AController* NewPlayer);
	void ServerCreateTeam(int32 TeamId, UGwTeamDisplayAsset* DisplayAsset);
	
	int32 GetLeastPopulatedTeamID() const;

};
