// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GwPawnData.generated.h"

class UGwAbilityTagRelationshipMapping;
class UGwInputConfig;
class UGwAbilitySet;
/**
 * 
 */
UCLASS(BlueprintType)
class GWMULTIPLERPG_API UGwPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UGwPawnData(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gw|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gw|Abilities")
	TArray<TObjectPtr<UGwAbilitySet>> AbilitySets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gw|Input")
	TObjectPtr<UGwInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gw|Abilities")
	TObjectPtr<UGwAbilityTagRelationshipMapping> TagRelationshipMapping;
};
