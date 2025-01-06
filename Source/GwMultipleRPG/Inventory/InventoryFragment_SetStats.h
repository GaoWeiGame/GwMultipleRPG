// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GwInventoryItemFragment.h"
#include "InventoryFragment_SetStats.generated.h"



UCLASS()
class GWMULTIPLERPG_API UInventoryFragment_SetStats : public UGwInventoryItemFragment
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TMap<FGameplayTag, int32> InitialItemStats;

public:
	virtual void OnInstanceCreated(UGwInventoryItemInstance* Instance) const override;

	int32 GetItemStatByTag(FGameplayTag Tag) const;
};
