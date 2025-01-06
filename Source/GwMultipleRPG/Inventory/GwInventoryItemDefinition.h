// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GwInventoryItemDefinition.generated.h"

class UGwInventoryItemFragment;
/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract)
class GWMULTIPLERPG_API UGwInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
public:
	const UGwInventoryItemFragment* FindFragmentByClass(TSubclassOf<UGwInventoryItemFragment> FragmentClass) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced)
	TArray<TObjectPtr<UGwInventoryItemFragment>> Fragments;
};
