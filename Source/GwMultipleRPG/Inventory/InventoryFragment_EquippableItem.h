// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwInventoryItemFragment.h"
#include "InventoryFragment_EquippableItem.generated.h"

class UGwEquipmentDefinition;
/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UInventoryFragment_EquippableItem : public UGwInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category=Gw)
	TSubclassOf<UGwEquipmentDefinition> EquipmentDefinition;
	
	
};
